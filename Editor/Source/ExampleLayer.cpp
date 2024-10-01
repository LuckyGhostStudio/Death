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
    m_CheckerboardTexture = std::make_shared<Lucky::Texture2D>("Assets/Textures/Checkerboard.png");    // 创建纹理

    Lucky::FramebufferSpecification fbSpec; // 帧缓冲区规范
    fbSpec.Width = 1280;
    fbSpec.Height = 720;
    m_Framebuffer = std::make_shared<Lucky::Framebuffer>(fbSpec);   // 创建帧缓冲区
}

void ExampleLayer::OnDetach()
{

}

void ExampleLayer::OnUpdate(Lucky::DeltaTime dt)
{
    fps = 1.0f / dt;

    m_CameraController.OnUpdate(dt);    // 更新相机控制器

    Lucky::Renderer2D::ResetStats();    // 重置统计数据

    m_Framebuffer->Bind();  // 绑定帧缓冲区

    Lucky::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
    Lucky::RenderCommand::Clear();

    

    Lucky::Renderer2D::BeginScene(m_CameraController.GetCamera());  // 开始渲染场景

    Lucky::Renderer2D::DrawQuad(m_SquarePosition, m_SquareRotation.z, m_SquareScale, m_SquareColor, m_CheckerboardTexture); // 绘制四边形
    Lucky::Renderer2D::DrawQuad({ -1.0f, 0.0f }, 0.0f, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 0.5f });     // 绘制四边形
    Lucky::Renderer2D::DrawQuad({ 0.5f, -0.5f }, 0.0f, { 0.5f, 0.75f }, { 0.2f, 0.8f, 0.3f, 1.0f });    // 绘制四边形

    Lucky::Renderer2D::EndScene();                                  // 结束渲染场景

    Lucky::Renderer2D::BeginScene(m_CameraController.GetCamera());  // 开始渲染场景

    for (float y = -5.0f; y < 5.0f; y += 0.5f)
    {
        for (float x = -5.0f; x < 5.0f; x += 0.5f)
        {
            glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.6f };
            Lucky::Renderer2D::DrawQuad({ x, y }, 0.0f, { 0.45f, 0.45f }, color);
        }
    }

    Lucky::Renderer2D::EndScene();  // 结束渲染场景

    m_Framebuffer->Unbind();        // 解除绑定帧缓冲区
}

void ExampleLayer::OnImGuiRender()
{
    static bool dockSpaceOpen = true;   // 停靠空间是否开启
    static bool opt_fullscreen_persistant = true;
    static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
    bool opt_fullscreen = opt_fullscreen_persistant;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruDockspace, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (opt_flags)
    {
        window_flags |= ImGuiWindowFlags_NoBackground;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags);	// 停靠空间
    {
        ImGui::PopStyleVar();
        if (opt_fullscreen)
        {
            ImGui::PopStyleVar(2);
        }

        // UI 停靠空间
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
        }

        // 菜单条
        if (ImGui::BeginMenuBar())
        {
            // 菜单：File
            if (ImGui::BeginMenu("File"))
            {
                // 菜单项：退出
                if (ImGui::MenuItem("Exit"))
                {
                    Lucky::Application::GetInstance().Close();	// 退出程序
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // 检视面板
        ImGui::Begin("Inspector");
        {
            ImGui::ColorEdit4("Color", glm::value_ptr(m_SquareColor));

            ImGui::SliderFloat3("Position", glm::value_ptr(m_SquarePosition), -10.0f, 10.0f);
            ImGui::SliderFloat3("Rotation", glm::value_ptr(m_SquareRotation), -360.0f, 360.0f);
            ImGui::SliderFloat3("Scale", glm::value_ptr(m_SquareScale), 0.0f, 10.0f);
        }
        ImGui::End();

        // 批渲染数据统计
        ImGui::Begin("Renderer2D Stats");
        {
            auto stats = Lucky::Renderer2D::GetStats();

            ImGui::Text("FPS: %.3f", fps);      // 帧率

            ImGui::Text("Draw Calls: %d", stats.DrawCalls);
            ImGui::Text("Quad: %d", stats.QuadCount);
            ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
            ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
        }
        ImGui::End();

        // 场景视口
        ImGui::Begin("Scene");
        {
            uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID(); // 颜色缓冲区 ID

            ImGui::Image((void*)textureID, ImVec2{ 720.0f, 405.0f });
        }
        ImGui::End();
    }
    ImGui::End();
}

void ExampleLayer::OnEvent(Lucky::Event& event)
{
    m_CameraController.OnEvent(event);  // 调用相机事件函数
}