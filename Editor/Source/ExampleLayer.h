#pragma once

#include <Lucky.h>

class ExampleLayer : public Lucky::Layer
{
private:
	std::shared_ptr<Lucky::Shader> m_Shader;			// ��ɫ��
	Lucky::ShaderLibrary m_ShaderLibrary;				// ��ɫ����

	std::shared_ptr<Lucky::VertexArray> m_VertexArray;	// ��������

	std::shared_ptr<Lucky::Shader> m_FlatColorShader;
	std::shared_ptr<Lucky::VertexArray> m_SquareVA;

	std::shared_ptr<Lucky::Texture2D> m_Texture;
	std::shared_ptr<Lucky::Texture2D> m_LuckyLogoTexture;

	Lucky::CameraController m_CameraController;			// ���������

	glm::vec3 m_SquareColor = { 0.2f,0.3f,0.8f };
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