#pragma once

#include "Core.h"
#include "Events/Event.h"

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