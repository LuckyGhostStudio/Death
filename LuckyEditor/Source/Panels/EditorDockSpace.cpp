#include "EditorDockSpace.h"

//#include <imgui/imgui.h>
//#include <imgui/imgui_internal.h>

namespace Lucky
{
    EditorDockSpace::EditorDockSpace(bool fullScreen)
        : m_IsFullScreen(fullScreen),
        m_Flags(ImGuiDockNodeFlags_None),
        m_WindowFlags(ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking)
    {

    }
}