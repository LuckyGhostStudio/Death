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
	m_CheckerboardTexture = std::make_shared<Lucky::Texture2D>("Assets/Textures/Checkerboard.png");	// 创建纹理
}

void ExampleLayer::OnDetach()
{

}

void ExampleLayer::OnUpdate(Lucky::DeltaTime dt)
{
	LC_TRACE("FPS: {0}", (int)(1000.0f / dt.GetMilliseconds()));

	Lucky::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Lucky::RenderCommand::Clear();

	m_CameraController.OnUpdate(dt);	// 更新相机控制器

	Lucky::Renderer2D::BeginScene(m_CameraController.GetCamera());			// 开始渲染场景

	Lucky::Renderer2D::DrawQuad(m_SquarePosition, m_SquareRotation.z, m_SquareScale, m_SquareColor, m_CheckerboardTexture);	// 绘制四边形
	Lucky::Renderer2D::DrawQuad({ -1.0f, 0.0f }, 0.0f, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });		// 绘制四边形
	Lucky::Renderer2D::DrawQuad({ 0.5f, -0.5f }, 0.0f, { 0.5f, 0.75f }, { 0.2f, 0.8f, 0.3f, 1.0f });	// 绘制四边形

	Lucky::Renderer2D::EndScene();						// 结束渲染场景
}

void ExampleLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");

	ImGui::ColorEdit4("Color", glm::value_ptr(m_SquareColor));

	ImGui::SliderFloat3("Position", glm::value_ptr(m_SquarePosition), -10.0f, 10.0f);
	ImGui::SliderFloat3("Rotation", glm::value_ptr(m_SquareRotation), -360.0f, 360.0f);
	ImGui::SliderFloat3("Scale", glm::value_ptr(m_SquareScale), 0.0f, 10.0f);

	ImGui::End();
}

void ExampleLayer::OnEvent(Lucky::Event& event)
{
	m_CameraController.OnEvent(event);		// 调用相机事件函数
}