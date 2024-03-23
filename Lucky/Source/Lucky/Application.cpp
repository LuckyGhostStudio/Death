#include "lcpch.h"
#include "Application.h"

#include <glad/glad.h>

namespace Lucky
{
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());					// 创建窗口
		m_Window->SetEventCallback(LC_BIND_EVENT_FUNC(Application::OnEvent));	// 设置回调函数
	}

	Application::~Application()
	{
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
		EventDispatcher dispatcher(e);		// 事件调度器
		dispatcher.Dispatch<WindowCloseEvent>(LC_BIND_EVENT_FUNC(Application::OnWindowClose));	// 调度 窗口关闭事件

		LC_CORE_TRACE("{0}", e.ToString());	// 事件触发日志

		// 从最顶层向下遍历层栈
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);		// 层获取并处理事件
			if (e.GetHandled()) break;	// 事件已处理
		}
	}

	void Application::Run()
	{
		while (m_Running) {
			glClearColor(0.3, 0.5, 0.8, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);

			// 更新层栈中所有层
			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}

			m_Window->OnUpdate();	// 更新窗口
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;	// 结束运行
		return true;
	}
}