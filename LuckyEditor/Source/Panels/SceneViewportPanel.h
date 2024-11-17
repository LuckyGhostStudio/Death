#pragma once

#include "EditorPanel.h"

#include "Lucky/Renderer/Framebuffer.h"
#include "Lucky/Renderer/EditorCamera.h"
#include "Lucky/Scene/Scene.h"
#include "Lucky/Scene/Selection.h"

#include "Lucky/Events/KeyEvent.h"
#include "Lucky/Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace Lucky
{
    /// <summary>
    /// 场景视口面板：渲染场景物体（编辑器相机的画面）
    /// </summary>
    class SceneViewportPanel : public EditorPanel
    {
    private:
        Ref<Framebuffer> m_Framebuffer;     // 帧缓冲区
        Ref<Scene> m_Scene;                 // 活动场景 会更新
        EditorCamera m_EditorCamera;        // 编辑器相机 会更新部分参数

        Object m_PickedObject;              // 被鼠标拾取的物体

        glm::vec2 m_Bounds[2];              // 视口边界（左上角，右下角）
        glm::vec2 m_Size = { 0.0f, 0.0f };  // 视口大小：随 m_Bounds 更新

        bool m_IsFocused = false;   // 视口被聚焦 +
        bool m_IsHovered = false;   // 视口被鼠标悬停 +

        int m_GizmoType = -1;       // Gizmo 操作类型 -1无 0平移 1旋转 2缩放 + TODO class Gizmo
    public:
        SceneViewportPanel() = default;
        SceneViewportPanel(const Ref<Framebuffer>& framebuffer, const Ref<Scene>& scene);

        virtual void SetSceneContext(const Ref<Scene>& scene) override;
        Ref<Scene> GetSceneContext() const { return m_Scene; }

        void OnUpdate(DeltaTime dt);

        virtual void OnImGuiRender(bool& isOpen) override;

        virtual void OnEvent(Event& e) override;
    private:
        /// <summary>
        /// 按键按下时调用
        /// </summary>
        /// <param name="e">按键按下事件</param>
        /// <returns>处理结果</returns>
        bool OnKeyPressed(KeyPressedEvent& e);

        /// <summary>
        /// 鼠标按钮按下时调用
        /// </summary>
        /// <param name="e">鼠标按钮按下事件</param>
        /// <returns>处理结果</returns>
        bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
    };
}