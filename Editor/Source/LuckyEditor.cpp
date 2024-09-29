#include <Lucky.h>
#include <Lucky/Core/EntryPoint.h>

#include "ExampleLayer.h"

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