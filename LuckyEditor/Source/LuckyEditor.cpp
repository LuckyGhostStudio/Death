#include <Lucky.h>
#include <Lucky/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Lucky
{
    class LuckyEditor : public Application
    {
    public:
        LuckyEditor(const ApplicationSpecification& spec)
            : Application(spec)
        {
            PushLayer(new EditorLayer());
        }

        ~LuckyEditor()
        {

        }
    };

    Application* CreateApplication(ApplicationCommandLineArgs args)
    {
        ApplicationSpecification spec;
        spec.Name = "Lucky Editor";
        spec.CommandLineArgs = args;

        return new LuckyEditor(spec);
    }
}