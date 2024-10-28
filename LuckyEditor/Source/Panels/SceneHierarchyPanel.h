#pragma once

#include "Lucky/Core/Core.h"
#include "Lucky/Core/Log.h"

#include "Lucky/Scene/Scene.h"
#include "Lucky/Scene/Object.h"

#include "Lucky/Scene/Selection.h"

namespace Lucky
{
    /// <summary>
    /// 场景物体层级面板
    /// </summary>
    class SceneHierarchyPanel
    {
    private:
        Ref<Scene> m_Scene;                             // 面板显示的场景
        Object& m_SelectionObject = Selection::Object;  // 当前选中项
    private:
        /// <summary>
        /// 绘制物体结点
        /// </summary>
        /// <param name="Object">物体</param>
        void DrawObjectNode(Object Object);

        /// <summary>
        /// 绘制物体的所有组件
        /// </summary>
        /// <param name="object">物体</param>
       //void DrawComponents(Object object);
    public:
        SceneHierarchyPanel() = default;

        /// <summary>
        /// 场景对象层级面板
        /// </summary>
        /// <param name="context">场景</param>
        SceneHierarchyPanel(const Ref<Scene>& scene);

        /// <summary>
        /// 设置场景
        /// </summary>
        /// <param name="context">场景</param>
        void SetScene(const Ref<Scene>& scene);

        /// <summary>
        /// 渲染ImGui
        /// </summary>
        void OnImGuiRender();
    };
}