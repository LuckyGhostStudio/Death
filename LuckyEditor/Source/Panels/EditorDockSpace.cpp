#include "EditorDockSpace.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Lucky
{
    EditorDockSpace::EditorDockSpace(bool fullScreen)
        : m_IsFullScreen(fullScreen),
        m_Flags(ImGuiDockNodeFlags_None),
        m_WindowFlags(ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking)
    {

    }

    void EditorDockSpace::Setup()
    {
        if (m_IsFullScreen)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();

            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);

            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

            m_WindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            m_WindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        if (m_Flags)
        {
            m_WindowFlags |= ImGuiWindowFlags_NoBackground;
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        // DockSpace ´°¿Ú
        ImGui::Begin("DockSpace", nullptr, m_WindowFlags);
        {
            ImGui::PopStyleVar();

            if (m_IsFullScreen)
            {
                ImGui::PopStyleVar(2);
            }

            ImGuiIO& io = ImGui::GetIO();

            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                // TODO ´´½¨ ToolBar
                ImGui::DockSpace(ImGui::GetID("EditorDockSpace"), ImVec2(0.0f, 0.0f), m_Flags | ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton);
            }
        }
        ImGui::End();
    }
}