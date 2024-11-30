#pragma once

#include "EditorPanel.h"

#include "Lucky/Scene/Scene.h"
#include "Lucky/Scene/Object.h"

#include "Lucky/Core/Events/KeyEvent.h"

namespace Lucky
{
    /// <summary>
    /// 场景物体层级面板：显示场景中的物体层次结构
    /// </summary>
    class SceneHierarchyPanel : public EditorPanel
    {
    private:
        Ref<Scene> m_Scene;         // 面板显示的场景
    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene>& scene);

        virtual void SetSceneContext(const Ref<Scene>& scene) override;
        Ref<Scene> GetSceneContext() const { return m_Scene; }

        virtual void OnImGuiRender(bool& isOpen) override;
    private:
        /// <summary>
        /// 绘制物体结点
        /// </summary>
        /// <param name="Object">物体</param>
        void DrawObjectNode(Object Object);

        /// <summary>
        /// 复制物体时调用
        /// </summary>
        void OnDuplicateObject();

        /// <summary>
        /// 删除物体时调用
        /// </summary>
        void OnDeleteObject();
    public:
        virtual void OnEvent(Event& event) override;
    private:
        /// <summary>
        /// 按键按下时调用
        /// </summary>
        /// <param name="e">按键按下事件</param>
        /// <returns>处理结果</returns>
        bool OnKeyPressed(KeyPressedEvent& e);
    };
}