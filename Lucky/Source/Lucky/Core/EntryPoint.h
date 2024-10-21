#pragma once

extern Lucky::Application* Lucky::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{
    Lucky::Log::Init();

    LC_CORE_WARN("Initialized Log!");
    LC_INFO("Hello, Lucky Engine!");

    auto app = Lucky::CreateApplication({ argc, argv });    // ´´½¨ Application
    
    app->Run();

    delete app;
}