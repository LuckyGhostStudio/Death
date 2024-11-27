#pragma once

#include "EditorPanel.h"

#include "Lucky/Renderer/Framebuffer.h"
#include "Lucky/Scene/Scene.h"

namespace Lucky
{
    /// <summary>
    /// ��Ϸ�ӿ���壺Main Camera ��Ⱦ�Ļ��� TODO ���һ�� ViewportPanel ���� ÿ�� Viewport ��Ҫһ�� Camera
    /// </summary>
    class GameViewportPanel : public EditorPanel
    {
    private:
        Ref<Framebuffer> m_Framebuffer;     // ֡������
        Ref<Scene> m_Scene;                 // ����� �����

        glm::vec2 m_Bounds[2];              // �ӿڱ߽磨���Ͻǣ����½ǣ�
        glm::vec2 m_Size = { 0.0f, 0.0f };  // �ӿڴ�С���� m_Bounds ����
    public:
        GameViewportPanel() = default;
        GameViewportPanel(const Ref<Framebuffer>& framebuffer, const Ref<Scene>& scene);

        virtual void SetSceneContext(const Ref<Scene>& scene) override;
        Ref<Scene> GetSceneContext() const { return m_Scene; }

        void OnUpdateEditor(DeltaTime dt);

        void OnUpdateRuntime(DeltaTime dt);

        virtual void OnImGuiRender(bool& isOpen) override;

        virtual void OnEvent(Event& e) override;
    };
}