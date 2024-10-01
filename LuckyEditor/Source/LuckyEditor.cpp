#include <Lucky.h>
#include <Lucky/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Lucky
{
    class LuckyEditor : public Application
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

    Application* CreateApplication()
    {
        return new LuckyEditor();
    }
}