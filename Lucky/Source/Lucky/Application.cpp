#include "lcpch.h"
#include "Application.h"

#include <glad/glad.h>

namespace Lucky
{
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());					// ��������
		m_Window->SetEventCallback(LC_BIND_EVENT_FUNC(Application::OnEvent));	// ���ûص�����
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
		EventDispatcher dispatcher(e);		// �¼�������
		dispatcher.Dispatch<WindowCloseEvent>(LC_BIND_EVENT_FUNC(Application::OnWindowClose));	// ���� ���ڹر��¼�

		LC_CORE_TRACE("{0}", e.ToString());	// �¼�������־

		// ��������±�����ջ
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);		// ���ȡ�������¼�
			if (e.GetHandled()) break;	// �¼��Ѵ���
		}
	}

	void Application::Run()
	{
		while (m_Running) {
			glClearColor(0.3, 0.5, 0.8, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);

			// ���²�ջ�����в�
			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}

			m_Window->OnUpdate();	// ���´���
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;	// ��������
		return true;
	}
}