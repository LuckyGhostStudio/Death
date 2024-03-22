#include "lcpch.h"
#include "Application.h"

#include "Lucky/Events/ApplicationEvent.h"

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

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);		// 事件调度器
		dispatcher.Dispatch<WindowCloseEvent>(LC_BIND_EVENT_FUNC(Application::OnWindowClose));	// 调度 窗口关闭事件

		LC_CORE_TRACE("{0}", e.ToString());	// 事件触发日志
	}

	void Application::Run()
	{
		while (m_Running) {
			glClearColor(0.3, 0.5, 0.8, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;	// 结束运行
		return true;
	}
}