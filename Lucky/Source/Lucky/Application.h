#pragma once

#include "Lucky/Core.h"
#include "Lucky/Events/Event.h"
#include "Lucky/Events/ApplicationEvent.h"
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

		/// <summary>
		/// �¼��ص�����
		/// </summary>
		/// <param name="e">�¼�</param>
		void OnEvent(Event& e);

		void Run();
	private:
		/// <summary>
		/// ���ڹرջص�����
		/// </summary>
		/// <param name="e">���ڹر��¼�</param>
		/// <returns>�Ƿ��ѹر�</returns>
		bool OnWindowClose(WindowCloseEvent& e);
	};

	/// <summary>
	/// ���� App���� LuckyEditor �ж���
	/// </summary>
	/// <returns></returns>
	Application* CreateApplication();
}