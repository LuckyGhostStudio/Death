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
	/// ���� App���� LuckyEditor �ж���
	/// </summary>
	/// <returns></returns>
	Application* CreateApplication();
}