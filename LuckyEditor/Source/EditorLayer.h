#pragma once

#include <Lucky.h>

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ObjectPropertiesPanel.h"

namespace Lucky
{
    class EditorLayer : public Layer
    {
    private:
        Ref<Framebuffer> m_Framebuffer;     // 帧缓冲区

        Ref<Scene> m_ActiveScene;           // 活动场景

        Object m_SquareObject1;             // 正方形物体
        Object m_SquareObject2;             // 正方形物体
        Object m_CameraObject;              // 相机对象

        bool m_ViewportFocused = false;     // 视口被聚焦
        bool m_ViewportHovered = false;     // 鼠标悬停在视口

        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };  // 视口大小

        SceneHierarchyPanel m_HierarchyPanel;       // 场景 Hierarchy 面板
        ObjectPropertiesPanel m_PropertiesPanel;    // 物体属性面板

        int m_GizmoType = -1;   // Gizmo 操作类型 -1无 0平移 1旋转 2缩放

        float fps = 0.0f;
    public:
        EditorLayer();

        virtual ~EditorLayer() = default;

        /// <summary>
        /// 将该层添加到层栈时调用
        /// </summary>
        virtual void OnAttach() override;

        /// <summary>
        /// 将该层从层栈移除时调用
        /// </summary>
        virtual void OnDetach() override;

        /// <summary>
        /// 更新：每帧调用
        /// </summary>
        /// <param name="dt">帧间隔</param>
        virtual void OnUpdate(DeltaTime dt) override;

        /// <summary>
        /// 渲染 ImGui
        /// </summary>
        virtual void OnImGuiRender() override;

        /// <summary>
        /// 事件处理函数
        /// </summary>
        /// <param name="event">事件</param>
        virtual void OnEvent(Event& event) override;

        /// <summary>
        /// 创建新场景
        /// </summary>
        void NewScene();

        /// <summary>
        /// 打开场景
        /// </summary>
        void OpenScene();

        /// <summary>
        /// 场景另存为
        /// </summary>
        void SaveSceneAs();
    private:
        /// <summary>
        /// 按键按下时调用
        /// </summary>
        /// <param name="e">按键按下事件</param>
        /// <returns>处理结果</returns>
        bool OnKeyPressed(KeyPressedEvent& e);
    };
}