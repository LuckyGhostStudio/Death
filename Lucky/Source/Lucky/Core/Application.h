#pragma once

#include "Lucky/Core/Core.h"
#include "Lucky/Core/Window.h"
#include "Lucky/Core/LayerStack.h"
#include "Lucky/Core/DeltaTime.h"

#include "Lucky/Events/Event.h"
#include "Lucky/Events/ApplicationEvent.h"

#include "Lucky/ImGui/ImGuiLayer.h"

namespace Lucky
{
    class Application
    {
    private:
        static Application* s_Instance; // ��̬ʵ��

        Scope<Window> m_Window;         // ����
        ImGuiLayer* m_ImGuiLayer;       // ImGui ��ָ��

        bool m_Running = true;          // �Ƿ���������
        bool m_Minimized = false;       // �Ƿ���С��

        LayerStack m_LayerStack;        // ��ջ

        float m_LastFrameTime = 0.0f;   // ��һ֡ʱ��
    public:
        Application(const std::string& name = "Lucky Editor");

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
        /// ����Application��ʵ��
        /// </summary>
        /// <returns>ʵ��</returns>
        inline static Application& GetInstance() { return *s_Instance; }

        /// <summary>
        /// ����Application�Ĵ���
        /// </summary>
        /// <returns></returns>
        inline Window& GetWindow() { return *m_Window; }

        /// <summary>
        /// ���� ImGui ��
        /// </summary>
        /// <returns>ImGui ��</returns>
        ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
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
    /// <returns></returns>
    Application* CreateApplication();
}