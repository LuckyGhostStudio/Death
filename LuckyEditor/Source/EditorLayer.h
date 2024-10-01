#pragma once

#include <Lucky.h>

namespace Lucky
{
    class EditorLayer : public Layer
    {
    private:
        ShaderLibrary m_ShaderLibrary;                      // 着色器库

        CameraController m_CameraController;                // 相机控制器

        std::shared_ptr<Texture2D> m_CheckerboardTexture;   // 棋盘纹理

        std::shared_ptr<Framebuffer> m_Framebuffer;         // 帧缓冲区

        bool m_ViewportFocused = false;     // 视口被聚焦
        bool m_ViewportHovered = false;     // 鼠标悬停在视口

        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };          // 视口大小

        glm::vec3 m_SquarePosition = { 0.0f, 0.0f, 0.0f };
        glm::vec3 m_SquareRotation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 m_SquareScale = { 1.0f, 1.0f, 1.0f };

        glm::vec4 m_SquareColor = { 0.2f,0.3f,0.8f, 1.0f };

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