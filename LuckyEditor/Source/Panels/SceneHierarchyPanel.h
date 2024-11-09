#pragma once

#include "EditorWindow.h"

#include "Lucky/Scene/Scene.h"
#include "Lucky/Scene/Object.h"

namespace Lucky
{
    /// <summary>
    /// 场景物体层级面板：显示场景中的物体层次结构
    /// </summary>
    class SceneHierarchyPanel : public EditorWindow
    {
    private:
        Ref<Scene> m_Scene; // 面板显示的场景
    private:
        /// <summary>
        /// 绘制物体结点
        /// </summary>
        /// <param name="Object">物体</param>
        void DrawObjectNode(Object Object);
    public:
        SceneHierarchyPanel();
        SceneHierarchyPanel(const Ref<Scene>& scene);
        
        virtual ~SceneHierarchyPanel();

        /// <summary>
        /// 设置场景
        /// </summary>
        /// <param name="scene">场景</param>
        void SetScene(const Ref<Scene>& scene);

        virtual void OnUpdate(DeltaTime dt) override;

        virtual void OnImGuiRender() override;

        virtual void OnEvent(Event& e) override;
    };
}