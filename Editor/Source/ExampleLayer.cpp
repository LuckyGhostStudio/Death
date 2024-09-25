#include "ExampleLayer.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>

ExampleLayer::ExampleLayer()
	: Layer("Example"), m_CameraController(1280.0f / 720.0f)
{
	
}

void ExampleLayer::OnAttach()
{
	
}

void ExampleLayer::OnDetach()
{

}

void ExampleLayer::OnUpdate(Lucky::DeltaTime dt)
{
	LC_TRACE("FPS: {0}", (int)(1000.0f / dt.GetMilliseconds()));

	Lucky::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Lucky::RenderCommand::Clear();

	m_CameraController.OnUpdate(dt);	// �������������

	Lucky::Renderer2D::BeginScene(m_CameraController.GetCamera());			// ��ʼ��Ⱦ����

	Lucky::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_SquareColor);	// �����ı���

	Lucky::Renderer2D::EndScene();						// ������Ⱦ����
}

void ExampleLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");

	ImGui::ColorEdit4("Triangle Color", glm::value_ptr(m_SquareColor));

	ImGui::End();
}

void ExampleLayer::OnEvent(Lucky::Event& event)
{
	m_CameraController.OnEvent(event);		// ��������¼�����
}