#include "lcpch.h"
#include "Application.h"

#include "Lucky/Events/ApplicationEvent.h"

#include <GLFW/glfw3.h>

namespace Lucky
{
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());	// ´´½¨´°¿Ú
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		// Test
		WindowResizeEvent e(1280, 720);

		if (e.IsInCategory(EventCategoryApplication)) {
			LC_TRACE(e.ToString());
		}
		if (e.IsInCategory(EventCategoryInput)) {
			LC_TRACE(e.ToString());
		}

		while (m_Running) {
			glClearColor(0.3, 0.5, 0.8, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}
}