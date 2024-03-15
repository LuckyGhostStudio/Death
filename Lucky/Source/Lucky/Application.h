#pragma once

#include "Core.h"

namespace Lucky {

	class Application
	{
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