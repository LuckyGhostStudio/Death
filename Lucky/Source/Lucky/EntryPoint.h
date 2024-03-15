#pragma once

#ifdef LC_PLATFORM_WINDOWS

extern Lucky::Application* Lucky::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Lucky::CreateApplication();
	app->Run();
	delete app;
}

#endif