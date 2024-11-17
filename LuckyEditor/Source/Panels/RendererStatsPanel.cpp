#include "RendererStatsPanel.h"

#include "Lucky/Renderer/Renderer2D.h"
#include "Lucky/Core/Application.h"

#include <imgui/imgui.h>

namespace Lucky
{
    void RendererStatsPanel::OnUpdate(DeltaTime dt)
    {
        Renderer2D::ResetStats();   // ����ͳ������
    }

    void RendererStatsPanel::OnImGuiRender(bool& isOpen)
    {
        ImGui::Begin("Renderer Stats"/*, &isOpen*/);
        {
            auto stats = Renderer2D::GetStats();    // ��Ⱦ������

            ImGui::Text("FPS: %.3f", Application::GetInstance().GetFPS());  // ֡��

            ImGui::Text("Draw Calls: %d", stats.DrawCalls);
            ImGui::Text("Quad: %d", stats.QuadCount);
            ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
            ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
        }
        ImGui::End();
    }
}