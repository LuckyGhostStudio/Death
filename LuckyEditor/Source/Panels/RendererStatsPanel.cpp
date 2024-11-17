#include "RendererStatsPanel.h"

#include "Lucky/Renderer/Renderer2D.h"
#include "Lucky/Core/Application.h"

#include <imgui/imgui.h>

namespace Lucky
{
    void RendererStatsPanel::OnUpdate(DeltaTime dt)
    {
        Renderer2D::ResetStats();   // 重置统计数据
    }

    void RendererStatsPanel::OnImGuiRender(bool& isOpen)
    {
        ImGui::Begin("Renderer Stats"/*, &isOpen*/);
        {
            auto stats = Renderer2D::GetStats();    // 渲染器数据

            ImGui::Text("FPS: %.3f", Application::GetInstance().GetFPS());  // 帧率

            ImGui::Text("Draw Calls: %d", stats.DrawCalls);
            ImGui::Text("Quad: %d", stats.QuadCount);
            ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
            ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
        }
        ImGui::End();
    }
}