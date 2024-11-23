#pragma once

#include "Lucky/Core/Core.h"
#include "Lucky/Events/Event.h"
#include "Lucky/Renderer/OpenGLContext.h"

#include <GLFW/glfw3.h>

namespace Lucky
{
    /// <summary>
    /// ��������
    /// </summary>
    struct WindowProps
    {
        std::string Title;      // ����
        unsigned int Width;     // ��
        unsigned int Height;    // ��

        WindowProps(const std::string& title = "Lucky Editor", unsigned int width = 1280, unsigned int height = 720)
            : Title(title), Width(width), Height(height) {}
    };

    /// <summary>
    /// ������
    /// </summary>
    class Window
    {
    public:
        using EventCallbackFn = std::function<void(Event&)>;    // �¼��ص����� void func(Event&)

        /// <summary>
        /// ��������
        /// </summary>
        /// <param name="props">��������</param>
        /// <returns>����</returns>
        static Scope<Window> Create(const WindowProps& props = WindowProps());

        /// <summary>
        /// ����
        /// </summary>
        /// <param name="props">��������</param>
        Window(const WindowProps& props);

        virtual ~Window();

        /// <summary>
        /// ���£�ÿ֡����
        /// </summary>
        void OnUpdate();

        inline unsigned int GetWidth() const { return m_Data.Width; }
        inline unsigned int GetHeight() const { return m_Data.Height; }

        /// <summary>
        /// �����¼��ص���������Application����
        /// </summary>
        /// <param name="callback">�ص�����</param>
        inline void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }

        /// <summary>
        /// ���ô�ֱͬ��
        /// </summary>
        /// <param name="enabled">�Ƿ�����</param>
        void SetVSync(bool enabled);

        /// <summary>
        /// ����Ƿ�ֱͬ��
        /// </summary>
        /// <returns>���</returns>
        bool IsVSync() const;

        /// <summary>
        /// ���� DPI
        /// </summary>
        /// <param name="dpi"></param>
        void SetDPI(unsigned int dpi);

        unsigned int GetDPI() const;

        /// <summary>
        /// ���� GLFW ����
        /// </summary>
        /// <returns>����ָ��</returns>
        inline virtual void* GetNativeWindow() const { return m_Window; }
    private:
        /// <summary>
        /// ��ʼ������
        /// </summary>
        /// <param name="props">��������</param>
        virtual void Init(const WindowProps& props);

        /// <summary>
        /// �رմ���
        /// </summary>
        virtual void Shutdown();
    private:
        GLFWwindow* m_Window;           // GLFW ����
        Scope<OpenGLContext> m_Context; // OpenGL ������

        /// <summary>
        /// ��������
        /// </summary>
        struct WindowData
        {
            std::string Title;      // ����
            unsigned int Width;     // ��
            unsigned int Height;    // ��
            unsigned DPI;           // DPI
            bool VSync;             // �Ƿ�ֱͬ��

            EventCallbackFn EventCallback;  // �¼��ص�����
        };

        WindowData m_Data;  // ��������
    };
}