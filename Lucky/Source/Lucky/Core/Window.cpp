#include "lcpch.h"
#include "Window.h"

#include "Lucky/Core/Events/ApplicationEvent.h"
#include "Lucky/Core/Events/KeyEvent.h"
#include "Lucky/Core/Events/MouseEvent.h"

namespace Lucky
{
    static bool s_GLFWInitialized = false;  // GLFW �Ƿ��ѳ�ʼ��

    /// <summary>
    /// GLFW ����ص�����
    /// </summary>
    /// <param name="error">��������</param>
    /// <param name="description">��������</param>
    static void GLFWErrorCallback(int error, const char* description)
    {
        LC_CORE_ERROR("GLFW Error ({0}): {1}", error, description); // ������־
    }

    Scope<Window> Window::Create(const WindowProps& props)
    {
        return CreateScope<Window>(props);
    }

    Window::Window(const WindowProps& props)
    {
        Init(props);
    }

    Window::~Window()
    {
        Shutdown();
    }

    void Window::Init(const WindowProps& props)
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        LC_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height); // �����־��Ϣ����������

        glfwSetErrorCallback(GLFWErrorCallback);    // ���ô���ص�����

        if (!s_GLFWInitialized) // δ��ʼ��
        {
            // TODO: glfwTerminate on system shutdown
            int success = glfwInit();                               // ��ʼ�� GLFW ����
            LC_CORE_ASSERT(success, "Could not initialize GLFW!");  // ��ʼ��ʧ�� ���ܳ�ʼ�� GLFW

            s_GLFWInitialized = true;
        }

        // ���� GLFW ����
        m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);

        m_Context = OpenGLContext::Create(m_Window);    // ���� OpenGL ������
        m_Context->Init();                              // ��ʼ��������

        glfwSetWindowUserPointer(m_Window, &m_Data);    // ������ m_Data ���ݸ� m_Window

        SetDPI(GetDpiForSystem());  // ���� DPI
        SetVSync(true);             // ��ֱͬ��

        LC_CORE_INFO("DPI: {0}", m_Data.DPI);
        
        /* ���� GLFW �Ļص����� */
        // ���ڴ�С�ı�
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);  // ͨ�� userPtr ָ���ȡ window ���ڵ� data ����

            // ���ڸ��º��С
            data.Width = width;
            data.Height = height;

            WindowResizeEvent event(width, height); // �������ڴ�С�ı��¼�
            data.EventCallback(event);              // �����¼��ص��������ص��������� Application ������Ϊ OnEvent��<=> OnEvent(event)
        });

        // ���ڹر�
        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            WindowCloseEvent event;     // ���ڹر��¼�
            data.EventCallback(event);
        });

        // ���̰������������� ����ɨ���� ��Ϊ �޸���λ��
        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action)
            {
                case GLFW_PRESS:    // ��������
                {
                    KeyPressedEvent event(key, 0);  // ���������¼�
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:  // ����̧��
                {
                    KeyReleasedEvent event(key);    // ����̧���¼�
                    data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT:   // �����ظ�
                {
                    KeyPressedEvent event(key, 1);  // ���������¼�
                    data.EventCallback(event);
                    break;
                }
            }
        });

        // �����ַ�����
        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            KeyTypedEvent event(keycode);   // ���������ַ��¼�
            data.EventCallback(event);
        });

        // ��갴ť
        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action)
            {
                case GLFW_PRESS:    // ��������
                {
                    MouseButtonPressedEvent event(button);  // ��갴ť�����¼�
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:  // ����̧��
                {
                    MouseButtonReleasedEvent event(button); // ��갴ţ̌���¼�
                    data.EventCallback(event);
                    break;
                }
            }
        });

        // �����ֹ���
        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseScrolledEvent event((float)xOffset, (float)yOffset);   // �����ֹ����¼�
            data.EventCallback(event);
        });

        // ����ƶ�
        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseMovedEvent event((float)xPos, (float)yPos);    // ����ƶ��¼�
            data.EventCallback(event);
        });
    }

    void Window::Shutdown()
    {
        glfwDestroyWindow(m_Window);    // ���ٴ���
        glfwTerminate();
    }

    void Window::OnUpdate()
    {
        glfwPollEvents();               // ��ѯ���д�������¼�
        m_Context->SwapBuffers();       // ����ǰ�󻺳���
    }

    void Window::SetVSync(bool enabled)
    {
        if (enabled)
        {
            glfwSwapInterval(1);        // �������Ϊ 1 ֡
        }
        else
        {
            glfwSwapInterval(0);
        }

        m_Data.VSync = enabled;
    }

    bool Window::IsVSync() const
    {
        return m_Data.VSync;
    }

    void Window::SetDPI(unsigned int dpi)
    {
        m_Data.DPI = dpi;
    }

    unsigned int Window::GetDPI() const
    {
        return m_Data.DPI;
    }
}