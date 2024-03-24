#include <Lucky.h>

class ExampleLayer : public Lucky::Layer
{
public:
	ExampleLayer() : Layer("Example") {}

	void OnUpdate() override
	{
		
	}

	void OnEvent(Lucky::Event& event) override
	{
		if (event.GetEventType() == Lucky::EventType::KeyPressed) {		// 按键按下事件
			Lucky::KeyPressedEvent& e = (Lucky::KeyPressedEvent&)event;
			LC_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};

class LuckyEditor : public Lucky::Application
{
public:
	LuckyEditor()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Lucky::ImGuiLayer());
	}

	~LuckyEditor()
	{

	}
};

Lucky::Application* Lucky::CreateApplication()
{
	return new LuckyEditor();
}