#include "GameViewportPanel.h"

#include "Lucky/Renderer/RenderCommand.h"
#include <imgui/imgui.h>

namespace Lucky
{
    GameViewportPanel::GameViewportPanel(const Ref<Framebuffer>& framebuffer, const Ref<Scene>& scene)
        : m_Framebuffer(framebuffer), m_Scene(scene)
    {

    }

    void GameViewportPanel::SetSceneContext(const Ref<Scene>& scene)
    {
        m_Scene = scene;
    }

    void GameViewportPanel::OnUpdate(DeltaTime dt)
    {
        if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
            m_Size.x > 0.0f && m_Size.y > 0.0f &&
            (spec.Width != m_Size.x || spec.Height != m_Size.y))
        {
            m_Framebuffer->Resize((uint32_t)m_Size.x, (uint32_t)m_Size.y);      // 重置帧缓冲区大小
            m_Scene->OnViewportResize((uint32_t)m_Size.x, (uint32_t)m_Size.y);  // 重置场景视口大小
        }

        m_Framebuffer->Bind();      // 绑定帧缓冲区

        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        RenderCommand::Clear();

        m_Framebuffer->ClearAttachment(1, -1);  // 清除颜色缓冲区 1（物体 id 缓冲区）为 -1

        m_Scene->OnUpdateRuntime(dt);   // 更新运行时场景

        m_Framebuffer->Unbind();        // 解除绑定帧缓冲区
    }

    void GameViewportPanel::OnImGuiRender(bool& isOpen)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1, 0)); // 设置 Gui 窗口样式：边界 = 0
        ImGui::Begin("Game");
        {
            m_IsFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();  // 当前面板大小
            m_Size = { viewportPanelSize.x, viewportPanelSize.y };      // 视口大小

            uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID(); // 颜色缓冲区 0 ID

            // Main Camera 视口图像
            ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_Size.x, m_Size.y }, ImVec2(0, 1), ImVec2(1, 0));
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }

    void GameViewportPanel::OnEvent(Event& e)
    {
    }
}
