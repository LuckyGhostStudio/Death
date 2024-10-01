#include <Lucky.h>
#include <Lucky/Core/EntryPoint.h>

#include "EditorLayer.h"

class LuckyEditor : public Lucky::Application
{
public:
    LuckyEditor()
    {
        PushLayer(new EditorLayer());
    }

    ~LuckyEditor()
    {

    }
};

Lucky::Application* Lucky::CreateApplication()
{
    return new LuckyEditor();
}