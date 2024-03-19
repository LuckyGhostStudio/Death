#include "Application.h"

#include "Events/ApplicationEvent.h"
#include "Log.h"

namespace Lucky
{
	Application::Application()
	{
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

		while (true);
	}
}