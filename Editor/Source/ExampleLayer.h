#pragma once

#include <Lucky.h>

class ExampleLayer : public Lucky::Layer
{
private:
    Lucky::ShaderLibrary m_ShaderLibrary;                       // ��ɫ����

    Lucky::CameraController m_CameraController;                 // ���������

    std::shared_ptr<Lucky::Texture2D> m_CheckerboardTexture;    // ��������

    glm::vec3 m_SquarePosition = { 0.0f, 0.0f, 0.0f };
    glm::vec3 m_SquareRotation = { 0.0f, 0.0f, 0.0f };
    glm::vec3 m_SquareScale = { 1.0f, 1.0f, 1.0f };

    glm::vec4 m_SquareColor = { 0.2f,0.3f,0.8f, 1.0f };

    float fps;
public:
    ExampleLayer();

    virtual ~ExampleLayer() = default;

    /// <summary>
    /// ���ò���ӵ���ջʱ����
    /// </summary>
    virtual void OnAttach() override;

    /// <summary>
    /// ���ò�Ӳ�ջ�Ƴ�ʱ����
    /// </summary>
    virtual void OnDetach() override;

    virtual void OnUpdate(Lucky::DeltaTime dt) override;

    virtual void OnImGuiRender() override;

    virtual void OnEvent(Lucky::Event& event) override;
};