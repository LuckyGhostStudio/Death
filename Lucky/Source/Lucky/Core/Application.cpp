#include "lcpch.h"
#include "Application.h"

#include "Lucky/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Lucky
{
    Application* Application::s_Instance = nullptr;

    Application::Application(const std::string& name, ApplicationCommandLineArgs args)
        : m_CommandLineArgs(args)
    {
        LC_CORE_ASSERT(!s_Instance, "Application already exists!");

        s_Instance = this;

        m_Window = Window::Create(WindowProps(name));      // ��������
        m_Window->SetEventCallback(LC_BIND_EVENT_FUNC(Application::OnEvent));   // ���ûص�����

        Renderer::Init();   // ��ʼ����Ⱦ��

        m_ImGuiLayer = new ImGuiLayer();    // ���� ImGui ��
        PushOverlay(m_ImGuiLayer);          // ��� ImGuiLayer �����ǲ�
    }

    Application::~Application()
    {
        Renderer::Shutdown();
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer* layer)
    {
        m_LayerStack.PushOverlay(layer);
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);  // �¼�������
        dispatcher.Dispatch<WindowCloseEvent>(LC_BIND_EVENT_FUNC(Application::OnWindowClose));      // ���� ���ڹر��¼�
        dispatcher.Dispatch<WindowResizeEvent>(LC_BIND_EVENT_FUNC(Application::OnWindowResize));    // ���� ���������¼�

        // ��������±�����ջ
        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            if (e.GetHandled())
            {
                break;  // �¼��Ѵ���
            }
            (*--it)->OnEvent(e);    // ���ȡ�������¼�
        }
    }

    void Application::Run()
    {
        while (m_Running)
        {
            float time = (float)glfwGetTime();              // ��ǰʱ��
            DeltaTime deltaTime = time - m_LastFrameTime;   // ֡��� = ��ǰʱ�� - ��һ֡ʱ��
            m_LastFrameTime = time;                         // ������һ֡ʱ��

            m_FramesPerSecond = 1.0f / deltaTime;           // ֡�� TODO ���� 1s ��ƽ��֡��

            // ����δ��С��
            if (!m_Minimized)
            {
                // ���²�ջ�����в�
                for (Layer* layer : m_LayerStack)
                {
                    layer->OnUpdate(deltaTime);
                }

                // ImGui��Ⱦ
                m_ImGuiLayer->Begin();
                for (Layer* layer : m_LayerStack)
                {
                    layer->OnImGuiRender(); // ��Ⱦÿ�� Layer �� ImGui
                }
                m_ImGuiLayer->End();
            }

            m_Window->OnUpdate();           // ���´���
        }
    }

    void Application::Close()
    {
        m_Running = false;  // ��������
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;  // ��������
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        if (e.GetWidth() == 0 || e.GetHeight() == 0)
        {
            m_Minimized = true;        // ������С��
            return false;
        }

        m_Minimized = false;
        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());  //���д������ź������
        // TODO ���� ImGui �� DisplaySize

        return false;
    }
}