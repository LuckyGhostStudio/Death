#include "EditorLayer.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>

namespace Lucky
{
    EditorLayer::EditorLayer()
        : Layer("Editor")
    {

    }

    void EditorLayer::OnAttach()
    {
        m_CheckerboardTexture = CreateRef<Texture2D>("Assets/Textures/Checkerboard.png");   // 创建纹理

        FramebufferSpecification fbSpec; // 帧缓冲区规范
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_Framebuffer = CreateRef<Framebuffer>(fbSpec); // 创建帧缓冲区

        m_ActiveScene = CreateRef<Scene>();             // 创建场景

        m_SquareObject = m_ActiveScene->CreateObject("Square");         // 创建正方形物体
        m_SquareObject.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));    // 添加 SpriteRenderer 组件

        m_CameraObject = m_ActiveScene->CreateObject("Main Camera");    // 创建相机对象
        m_CameraObject.AddComponent<CameraComponent>();                 // 添加 Camera 组件

        m_HierarchyPanel.SetScene(m_ActiveScene);   // 设置 Hierarchy 面板的场景
    }

    void EditorLayer::OnDetach()
    {

    }

    void EditorLayer::OnUpdate(DeltaTime dt)
    {
        fps = 1.0f / dt;

        if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);  // 重置帧缓冲区大小

            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);    // 重置视口大小
        }

        // 视口被聚焦
        if (m_ViewportFocused)
        {

        }

        Renderer2D::ResetStats();   // 重置统计数据

        m_Framebuffer->Bind();      // 绑定帧缓冲区

        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        RenderCommand::Clear();

        m_ActiveScene->OnUpdate(dt);    // 更新场景

        m_Framebuffer->Unbind();    // 解除绑定帧缓冲区
    }

    void EditorLayer::OnImGuiRender()
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
        if (opt_flags /*& &ImGuiDockNodeFlags_PassthruDockspace*/)
        {
            window_flags |= ImGuiWindowFlags_NoBackground;
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        // 停靠空间
        ImGui::Begin("DockSpace", &dockSpaceOpen, window_flags);
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
                ImGuiID dockspace_id = ImGui::GetID("Editor Dockspace");
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
                        Application::GetInstance().Close();	// 退出程序
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            m_HierarchyPanel.OnImGuiRender();   // 渲染 Hierarchy 面板

            // 检视面板
            ImGui::Begin("Inspector");
            {
                auto& squarePos = m_SquareObject.GetComponent<TransformComponent>().Position;   // 位置
                auto& squareRot = m_SquareObject.GetComponent<TransformComponent>().Rotation;   // 旋转
                auto& squareScale = m_SquareObject.GetComponent<TransformComponent>().Scale;    // 缩放
                
                auto& squareColor = m_SquareObject.GetComponent<SpriteRendererComponent>().Color;   // 颜色

                ImGui::DragFloat3("Position", glm::value_ptr(squarePos));
                ImGui::DragFloat3("Rotation", glm::value_ptr(squareRot));
                ImGui::DragFloat3("Scale", glm::value_ptr(squareScale));

                ImGui::ColorEdit4("Color", glm::value_ptr(squareColor));

                ImGui::Separator();

                auto& camera = m_CameraObject.GetComponent<CameraComponent>().Camera;
                float cameraSize = camera.GetSize();        // 尺寸
                float cameraNear = camera.GetNearClip();    // 近裁剪平面
                float cameraFar = camera.GetFarClip();      // 远裁剪平面

                ImGui::Separator();

                ImGui::Text("Camera");
                if (ImGui::DragFloat("Size", &cameraSize)) {
                    camera.SetSize(cameraSize);
                }
                if (ImGui::DragFloat("Near", &cameraNear)) {
                    camera.SetNearClip(cameraNear);
                }
                if (ImGui::DragFloat("Far", &cameraFar)) {
                    camera.SetFarClip(cameraFar);
                }
            }
            ImGui::End();

            // 批渲染数据统计
            ImGui::Begin("Renderer2D Stats");
            {
                auto stats = Renderer2D::GetStats();

                ImGui::Text("FPS: %.3f", fps);      // 帧率

                ImGui::Text("Draw Calls: %d", stats.DrawCalls);
                ImGui::Text("Quad: %d", stats.QuadCount);
                ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
                ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
            }
            ImGui::End();

            // 场景视口
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0)); // 设置 Gui 窗口样式：边界 = 0
            ImGui::Begin("Scene");
            {
                m_ViewportFocused = ImGui::IsWindowFocused();   // 当前窗口被聚焦
                m_ViewportHovered = ImGui::IsWindowHovered();   // 鼠标悬停在当前窗口

                Application::GetInstance().GetImGuiLayer()->BlockEvents(/*!m_ViewportFocused ||*/ !m_ViewportHovered); // 阻止ImGui事件

                ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();      // 当前面板大小

                m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };  // 视口大小

                uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID(); // 颜色缓冲区 ID

                ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));   // 场景视口图像
            }
            ImGui::End();
            ImGui::PopStyleVar();
        }
        ImGui::End();
    }

    void EditorLayer::OnEvent(Event& event)
    {

    }
}