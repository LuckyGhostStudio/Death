#pragma once

#include "Lucky/Core/Core.h"
#include "Lucky/Core/Window.h"
#include "Lucky/Core/LayerStack.h"
#include "Lucky/Core/DeltaTime.h"

#include "Lucky/Core/Events/Event.h"
#include "Lucky/Core/Events/ApplicationEvent.h"

#include "Lucky/ImGui/ImGuiLayer.h"

namespace Lucky
{
    /// <summary>
    /// Application �����в���
    /// </summary>
    struct ApplicationCommandLineArgs
    {
        int Count = 0;          // ��������
        char** Args = nullptr;  // �����б�

        const char* operator[](int index) const
        {
            LC_CORE_ASSERT(index < Count, "Index �����˲���������");

            return Args[index];
        }
    };

    /// <summary>
    /// Application �淶
    /// </summary>
    struct ApplicationSpecification
    {
        std::string Name = "Lucky Application";
        std::string WorkingDirectory;               // ��Ŀ����Ŀ¼

        ApplicationCommandLineArgs CommandLineArgs; // �����в���
    };

    class Application
    {
    private:
        static Application* s_Instance; // ��̬ʵ��

        ApplicationSpecification m_Specification;   // App �淶
        
        Scope<Window> m_Window;         // ����
        ImGuiLayer* m_ImGuiLayer;       // ImGui ��ָ��

        bool m_Running = true;          // �Ƿ���������
        bool m_Minimized = false;       // �Ƿ���С��

        LayerStack m_LayerStack;        // ��ջ

        float m_LastFrameTime = 0.0f;   // ��һ֡ʱ��
        float m_FramesPerSecond = 0.0f; // ֡��
    public:
        Application(const ApplicationSpecification& specification);

        virtual ~Application();

        /// <summary>
        /// �����ͨ�㵽��ջ
        /// </summary>
        /// <param name="layer">��ͨ��</param>
        void PushLayer(Layer* layer);

        /// <summary>
        /// ��Ӹ��ǲ㵽��ջ
        /// </summary>
        /// <param name="layer">���ǲ�</param>
        void PushOverlay(Layer* layer);

        /// <summary>
        /// �¼��ص�����
        /// </summary>
        /// <param name="e">�¼�</param>
        void OnEvent(Event& e);

        /// <summary>
        /// ���У���ѭ��
        /// </summary>
        void Run();

        /// <summary>
        /// �رճ���
        /// </summary>
        void Close();

        /// <summary>
        /// ����֡��
        /// </summary>
        /// <returns>֡��</returns>
        inline float GetFPS() const { return m_FramesPerSecond; }

        /// <summary>
        /// ���� Application ��ʵ��
        /// </summary>
        /// <returns>ʵ��</returns>
        inline static Application& GetInstance() { return *s_Instance; }

        /// <summary>
        /// ���� Application �Ĵ���
        /// </summary>
        /// <returns></returns>
        inline Window& GetWindow() { return *m_Window; }

        /// <summary>
        /// ���� ImGui ��
        /// </summary>
        /// <returns>ImGui ��</returns>
        ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

        const ApplicationSpecification& GetSpecification() const { return m_Specification; }
    private:
        /// <summary>
        /// ���ڹرջص�����
        /// </summary>
        /// <param name="e">���ڹر��¼�</param>
        /// <returns>�Ƿ��ѹر�</returns>
        bool OnWindowClose(WindowCloseEvent& e);

        /// <summary>
        /// ��������ʱ����
        /// </summary>
        /// <param name="e">���������¼�</param>
        /// <returns>�¼�������</returns>
        bool OnWindowResize(WindowResizeEvent& e);
    };

    /// <summary>
    /// ���� App���� LuckyEditor �ж���
    /// </summary>
    /// <param name="args">�����в���</param>
    /// <returns></returns>
    Application* CreateApplication(ApplicationCommandLineArgs args);
}