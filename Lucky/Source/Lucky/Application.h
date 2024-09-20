#pragma once

#include "Lucky/Core.h"
#include "Lucky/LayerStack.h"
#include "Lucky/Events/Event.h"
#include "Lucky/Events/ApplicationEvent.h"
#include "Lucky/Window.h"

#include "Lucky/ImGui/ImGuiLayer.h"

#include "Lucky/Renderer/Buffer.h"
#include "Lucky/Renderer/Shader.h"

namespace Lucky
{
	class Application
	{
	private:
		static Application* s_Instance;		// 静态实例

		std::unique_ptr<Window> m_Window;	// 窗口指针
		ImGuiLayer* m_ImGuiLayer;			// ImGui层指针

		bool m_Running = true;				// 是否正在运行
		LayerStack m_LayerStack;			// 层栈

		unsigned int m_VertexArray;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;	// 顶点缓冲区
		std::unique_ptr<IndexBuffer> m_IndexBuffer;		// 索引缓冲区
		std::unique_ptr<Shader> m_Shader;				// 着色器
	public:
		Application();
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

		void Run();

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
	private:
		/// <summary>
		/// 窗口关闭回调函数
		/// </summary>
		/// <param name="e">窗口关闭事件</param>
		/// <returns>是否已关闭</returns>
		bool OnWindowClose(WindowCloseEvent& e);
	};

	/// <summary>
	/// 创建 App：在 LuckyEditor 中定义
	/// </summary>
	/// <returns></returns>
	Application* CreateApplication();
}