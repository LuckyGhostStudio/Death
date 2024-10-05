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
        static Application* s_Instance; // 静态实例

        Scope<Window> m_Window;         // 窗口
        ImGuiLayer* m_ImGuiLayer;       // ImGui 层指针

        bool m_Running = true;          // 是否正在运行
        bool m_Minimized = false;       // 是否最小化

        LayerStack m_LayerStack;        // 层栈

        float m_LastFrameTime = 0.0f;   // 上一帧时间
    public:
        Application(const std::string& name = "Lucky Editor");

        virtual ~Application();

        /// <summary>
        /// 添加普通层到层栈
        /// </summary>
        /// <param name="layer">普通层</param>
        void PushLayer(Layer* layer);

        /// <summary>
        /// 添加覆盖层到层栈
        /// </summary>
        /// <param name="layer">覆盖层</param>
        void PushOverlay(Layer* layer);

        /// <summary>
        /// 事件回调函数
        /// </summary>
        /// <param name="e">事件</param>
        void OnEvent(Event& e);

        /// <summary>
        /// 运行：主循环
        /// </summary>
        void Run();

        /// <summary>
        /// 关闭程序
        /// </summary>
        void Close();

        /// <summary>
        /// 返回Application的实例
        /// </summary>
        /// <returns>实例</returns>
        inline static Application& GetInstance() { return *s_Instance; }

        /// <summary>
        /// 返回Application的窗口
        /// </summary>
        /// <returns></returns>
        inline Window& GetWindow() { return *m_Window; }

        /// <summary>
        /// 返回 ImGui 层
        /// </summary>
        /// <returns>ImGui 层</returns>
        ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
    private:
        /// <summary>
        /// 窗口关闭回调函数
        /// </summary>
        /// <param name="e">窗口关闭事件</param>
        /// <returns>是否已关闭</returns>
        bool OnWindowClose(WindowCloseEvent& e);

        /// <summary>
        /// 窗口缩放时调用
        /// </summary>
        /// <param name="e">窗口缩放事件</param>
        /// <returns>事件处理结果</returns>
        bool OnWindowResize(WindowResizeEvent& e);
    };

    /// <summary>
    /// 创建 App：在 LuckyEditor 中定义
    /// </summary>
    /// <returns></returns>
    Application* CreateApplication();
}