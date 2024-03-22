#include <Lucky.h>

class ExampleLayer : public Lucky::Layer
{
public:
	ExampleLayer() : Layer("Example") {}

	void OnUpdate() override
	{
		LC_INFO("ExampleLayer::OnUpdate");
	}

	void OnEvent(Lucky::Event& event) override
	{
		LC_TRACE("{0}", event.ToString());
	}
};

class LuckyEditor : public Lucky::Application
{
public:
	LuckyEditor()
	{
		PushLayer(new ExampleLayer());
	}

	~LuckyEditor()
	{

	}
};

Lucky::Application* Lucky::CreateApplication()
{
	return new LuckyEditor();
}