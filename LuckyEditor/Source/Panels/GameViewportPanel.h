#pragma once

#include "EditorPanel.h"

#include "Lucky/Renderer/Framebuffer.h"
#include "Lucky/Scene/Scene.h"

namespace Lucky
{
    /// <summary>
    /// 游戏视口面板：Main Camera 渲染的画面 TODO 添加一个 ViewportPanel 基类 每个 Viewport 需要一个 Camera
    /// </summary>
    class GameViewportPanel : public EditorPanel
    {
    private:
        Ref<Framebuffer> m_Framebuffer;     // 帧缓冲区
        Ref<Scene> m_Scene;                 // 活动场景 会更新

        glm::vec2 m_Bounds[2];              // 视口边界（左上角，右下角）
        glm::vec2 m_Size = { 0.0f, 0.0f };  // 视口大小：随 m_Bounds 更新
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