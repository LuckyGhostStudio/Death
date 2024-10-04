#pragma once

#include <Lucky.h>

namespace Lucky
{
    class EditorLayer : public Layer
    {
    private:
        ShaderLibrary m_ShaderLibrary;                      // 着色器库

        std::shared_ptr<Texture2D> m_CheckerboardTexture;   // 棋盘纹理

        std::shared_ptr<Framebuffer> m_Framebuffer;         // 帧缓冲区

        std::shared_ptr<Scene> m_ActiveScene;   // 活动场景
        Object m_SquareObject;                  // 正方形物体
        Object m_CameraObject;                  // 相机对象

        bool m_ViewportFocused = false;     // 视口被聚焦
        bool m_ViewportHovered = false;     // 鼠标悬停在视口

        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };  // 视口大小

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
    };
}