#pragma once

#include "EditorPanel.h"

#include "Lucky/Scene/Scene.h"
#include "Lucky/Scene/Object.h"

namespace Lucky
{
    /// <summary>
    /// 场景物体层级面板：显示场景中的物体层次结构
    /// </summary>
    class SceneHierarchyPanel : public EditorPanel
    {
    private:
        Ref<Scene> m_Scene;         // 面板显示的场景
    private:
        /// <summary>
        /// 绘制物体结点
        /// </summary>
        /// <param name="Object">物体</param>
        void DrawObjectNode(Object Object);
    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene>& scene);

        virtual void SetSceneContext(const Ref<Scene>& scene) override;
        Ref<Scene> GetSceneContext() const { return m_Scene; }

        virtual void OnImGuiRender(bool& isOpen) override;

        virtual void OnEvent(Event& event) override;
    };
}