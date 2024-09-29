#pragma once

#include <Lucky.h>

class ExampleLayer : public Lucky::Layer
{
private:
    Lucky::ShaderLibrary m_ShaderLibrary;                       // 着色器库

    Lucky::CameraController m_CameraController;                 // 相机控制器

    std::shared_ptr<Lucky::Texture2D> m_CheckerboardTexture;    // 棋盘纹理

    glm::vec3 m_SquarePosition = { 0.0f, 0.0f, 0.0f };
    glm::vec3 m_SquareRotation = { 0.0f, 0.0f, 0.0f };
    glm::vec3 m_SquareScale = { 1.0f, 1.0f, 1.0f };

    glm::vec4 m_SquareColor = { 0.2f,0.3f,0.8f, 1.0f };

    float fps;
public:
    ExampleLayer();

    virtual ~ExampleLayer() = default;

    /// <summary>
    /// 将该层添加到层栈时调用
    /// </summary>
    virtual void OnAttach() override;

    /// <summary>
    /// 将该层从层栈移除时调用
    /// </summary>
    virtual void OnDetach() override;

    virtual void OnUpdate(Lucky::DeltaTime dt) override;

    virtual void OnImGuiRender() override;

    virtual void OnEvent(Lucky::Event& event) override;
};