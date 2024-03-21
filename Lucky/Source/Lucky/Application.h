#pragma once

#include "Lucky/Core.h"
#include "Lucky/Events/Event.h"
#include "Lucky/Window.h"

namespace Lucky {

	class Application
	{
	private:
		std::unique_ptr<Window> m_Window;	// 窗口指针
		bool m_Running = true;				// 是否正在运行
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	/// <summary>
	/// 创建 App：在 LuckyEditor 中定义
	/// </summary>
	/// <returns></returns>
	Application* CreateApplication();
}