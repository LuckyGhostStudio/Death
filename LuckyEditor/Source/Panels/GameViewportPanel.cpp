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
            m_Framebuffer->Resize((uint32_t)m_Size.x, (uint32_t)m_Size.y);      // ����֡��������С
            m_Scene->OnViewportResize((uint32_t)m_Size.x, (uint32_t)m_Size.y);  // ���ó����ӿڴ�С
        }

        m_Framebuffer->Bind();      // ��֡������

        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        RenderCommand::Clear();

        m_Framebuffer->ClearAttachment(1, -1);  // �����ɫ������ 1������ id ��������Ϊ -1

        m_Scene->OnUpdateRuntime(dt);   // ��������ʱ����

        m_Framebuffer->Unbind();        // �����֡������
    }

    void GameViewportPanel::OnImGuiRender(bool& isOpen)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1, 0)); // ���� Gui ������ʽ���߽� = 0
        ImGui::Begin("Game");
        {
            m_IsFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();  // ��ǰ����С
            m_Size = { viewportPanelSize.x, viewportPanelSize.y };      // �ӿڴ�С

            uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID(); // ��ɫ������ 0 ID

            // Main Camera �ӿ�ͼ��
            ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_Size.x, m_Size.y }, ImVec2(0, 1), ImVec2(1, 0));
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }

    void GameViewportPanel::OnEvent(Event& e)
    {
    }
}
