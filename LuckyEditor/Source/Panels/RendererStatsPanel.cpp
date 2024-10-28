#include "RendererStatsPanel.h"

#include "Lucky/Renderer/Renderer2D.h"
#include "Lucky/Core/Application.h"

#include <imgui/imgui.h>

namespace Lucky
{
    RendererStatsPanel::RendererStatsPanel()
        : EditorWindow("Renderer Stats")
    {

    }

    RendererStatsPanel::~RendererStatsPanel()
    {

    }

    void RendererStatsPanel::OnImGuiRender()
    {
        ImGui::Begin(m_Name.c_str());
        {
            auto stats = Renderer2D::GetStats();    // äÖÈ¾Æ÷Êý¾Ý

            ImGui::Text("FPS: %.3f", Application::GetInstance().GetFPS());  // Ö¡ÂÊ

            ImGui::Text("Draw Calls: %d", stats.DrawCalls);
            ImGui::Text("Quad: %d", stats.QuadCount);
            ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
            ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
        }
        ImGui::End();
    }
}