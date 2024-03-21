#pragma once

#include "Lucky/Core.h"
#include "Lucky/Events/Event.h"
#include "Lucky/Window.h"

namespace Lucky {

	class Application
	{
	private:
		std::unique_ptr<Window> m_Window;	// ����ָ��
		bool m_Running = true;				// �Ƿ���������
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