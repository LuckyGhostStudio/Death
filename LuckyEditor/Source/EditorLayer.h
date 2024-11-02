#pragma once

#include <Lucky.h>

#include "Lucky/Renderer/EditorCamera.h"

#include "Panels/EditorDockSpace.h"
#include "Panels/SceneViewportPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/InspectorPanel.h"
#include "Panels/RendererStatsPanel.h"

namespace Lucky
{
    class EditorLayer : public Layer
    {
    private:
        Ref<Framebuffer> m_Framebuffer;     // 帧缓冲区

        Ref<Scene> m_ActiveScene;           // 活动场景
        EditorCamera m_EditorCamera;        // 编辑器相机

        Object m_SquareObject;              // 正方形物体
        Object m_CameraObject;              // 相机对象

        EditorDockSpace m_EditorDockSpace;          // 停靠空间

        SceneViewportPanel m_SceneViewportPanel;    // 场景 Viewport 面板
        SceneHierarchyPanel m_SceneHierarchyPanel;  // 场景 Hierarchy 面板
        InspectorPanel m_InspectorPanel;            // 资产 Inspector 面板
        RendererStatsPanel m_RendererStatsPanel;    // 场景 RendererStats 面板
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

        /// <summary>
        /// 鼠标按钮按下时调用
        /// </summary>
        /// <param name="e">鼠标按钮按下事件</param>
        /// <returns>处理结果</returns>
        bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
    };
}