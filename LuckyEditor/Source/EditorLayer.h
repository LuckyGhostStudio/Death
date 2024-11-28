#pragma once

#include <Lucky.h>

#include "Lucky/Renderer/EditorCamera.h"

#include "Panels/EditorDockSpace.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/InspectorPanel.h"
#include "Panels/SceneViewportPanel.h"
#include "Panels/GameViewportPanel.h"
#include "Panels/RendererStatsPanel.h"
#include "Panels/ProjectAssetsPanel.h"

namespace Lucky
{
    class EditorLayer : public Layer
    {
    private:
        Ref<Framebuffer> m_SceneViewportFramebuffer;    // 场景视口帧缓冲区
        Ref<Framebuffer> m_GameViewportFramebuffer;     // 游戏视口帧缓冲区

        Ref<Scene> m_CurrentScene;              // 当前场景
        Ref<Scene> m_EditorScene;               // 编辑时场景
        Ref<Scene> m_RuntimeScene;              // 运行时场景
        std::filesystem::path m_SceneFilePath;  // 场景文件路径

        EditorCamera m_EditorCamera;    // 编辑器相机

        Object m_CameraObject;          // 相机对象

        /// <summary>
        /// 场景状态
        /// </summary>
        enum class SceneState
        {
            Edit = 0, Play = 1
        };
        SceneState m_SceneState = SceneState::Edit;

        EditorDockSpace m_EditorDockSpace;  // 停靠空间

        // TODO Add PanelManager
        Ref<SceneHierarchyPanel> m_SceneHierarchyPanel; // 场景 Hierarchy 面板
        Ref<InspectorPanel> m_InspectorPanel;           // 资产 Inspector 面板
        Ref<SceneViewportPanel> m_SceneViewportPanel;   // 场景 Viewport 面板
        Ref<GameViewportPanel> m_GameViewportPanel;     // 游戏 Viewport 面板
        Ref<RendererStatsPanel> m_RendererStatsPanel;   // 场景 RendererStats 面板
        Ref<ProjectAssetsPanel> m_ProjectAssetsPanel;   // 项目 Assets 面板

        Ref<Texture2D> m_PlayButtonIcon;    // 运行按钮

        float m_MenuBarHeight;
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
    private:
        /// <summary>
        /// 菜单条
        /// </summary>
        void UI_MenuBar();

        /// <summary>
        /// 工具栏
        /// </summary>
        void UI_ToolBar();

        /// <summary>
        /// 场景开始运行时调用
        /// </summary>
        void OnScenePlay();

        /// <summary>
        /// 场景停止运行时调用
        /// </summary>
        void OnSceneStop();

        /// <summary>
        /// 创建新场景
        /// </summary>
        void NewScene();

        /// <summary>
        /// 打开场景
        /// </summary>
        void OpenScene();

        /// <summary>
        /// 打开场景
        /// </summary>
        /// <param name="filepath">文件路径</param>
        void OpenScene(const std::filesystem::path& filepath);

        /// <summary>
        /// 保存场景到当前场景
        /// </summary>
        void SaveScene();

        /// <summary>
        /// 场景另存为
        /// </summary>
        void SaveSceneAs();

        /// <summary>
        /// 序列化场景
        /// </summary>
        /// <param name="scene">场景</param>
        /// <param name="path">路径</param>
        void SerializeScene(Ref<Scene> scene, const std::filesystem::path& filepath);
    public:
        /// <summary>
        /// 事件处理函数
        /// </summary>
        /// <param name="event">事件</param>
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