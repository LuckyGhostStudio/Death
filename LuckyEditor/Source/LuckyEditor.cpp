#include <Lucky.h>
#include <Lucky/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Lucky
{
    class LuckyEditor : public Application
    {
    public:
        LuckyEditor(ApplicationCommandLineArgs args)
            : Application("Lucky Editor", args)
        {
            PushLayer(new EditorLayer());
        }

        ~LuckyEditor()
        {

        }
    };

    Application* CreateApplication(ApplicationCommandLineArgs args)
    {
        return new LuckyEditor(args);
    }
}