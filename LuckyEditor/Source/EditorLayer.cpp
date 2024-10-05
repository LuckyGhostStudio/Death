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
        m_CheckerboardTexture = CreateRef<Texture2D>("Assets/Textures/Checkerboard.png");   // ��������

        FramebufferSpecification fbSpec; // ֡�������淶
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_Framebuffer = CreateRef<Framebuffer>(fbSpec); // ����֡������

        m_ActiveScene = CreateRef<Scene>();             // ��������

        m_SquareObject = m_ActiveScene->CreateObject("Square");         // ��������������
        m_SquareObject.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));    // ��� SpriteRenderer ���

        m_CameraObject = m_ActiveScene->CreateObject("Main Camera");    // �����������
        m_CameraObject.AddComponent<CameraComponent>();                 // ��� Camera ���

        m_HierarchyPanel.SetScene(m_ActiveScene);   // ���� Hierarchy ���ĳ���
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
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);  // ����֡��������С

            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);    // �����ӿڴ�С
        }

        // �ӿڱ��۽�
        if (m_ViewportFocused)
        {

        }

        Renderer2D::ResetStats();   // ����ͳ������

        m_Framebuffer->Bind();      // ��֡������

        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        RenderCommand::Clear();

        m_ActiveScene->OnUpdate(dt);    // ���³���

        m_Framebuffer->Unbind();    // �����֡������
    }

    void EditorLayer::OnImGuiRender()
    {
        static bool dockSpaceOpen = true;   // ͣ���ռ��Ƿ���
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

        // ͣ���ռ�
        ImGui::Begin("DockSpace", &dockSpaceOpen, window_flags);
        {
            ImGui::PopStyleVar();
            if (opt_fullscreen)
            {
                ImGui::PopStyleVar(2);
            }

            // UI ͣ���ռ�
            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("Editor Dockspace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
            }

            // �˵���
            if (ImGui::BeginMenuBar())
            {
                // �˵���File
                if (ImGui::BeginMenu("File"))
                {
                    // �˵���˳�
                    if (ImGui::MenuItem("Exit"))
                    {
                        Application::GetInstance().Close();	// �˳�����
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            m_HierarchyPanel.OnImGuiRender();   // ��Ⱦ Hierarchy ���

            // �������
            ImGui::Begin("Inspector");
            {
                auto& squarePos = m_SquareObject.GetComponent<TransformComponent>().Position;   // λ��
                auto& squareRot = m_SquareObject.GetComponent<TransformComponent>().Rotation;   // ��ת
                auto& squareScale = m_SquareObject.GetComponent<TransformComponent>().Scale;    // ����
                
                auto& squareColor = m_SquareObject.GetComponent<SpriteRendererComponent>().Color;   // ��ɫ

                ImGui::DragFloat3("Position", glm::value_ptr(squarePos));
                ImGui::DragFloat3("Rotation", glm::value_ptr(squareRot));
                ImGui::DragFloat3("Scale", glm::value_ptr(squareScale));

                ImGui::ColorEdit4("Color", glm::value_ptr(squareColor));

                ImGui::Separator();

                auto& camera = m_CameraObject.GetComponent<CameraComponent>().Camera;
                float cameraSize = camera.GetSize();        // �ߴ�
                float cameraNear = camera.GetNearClip();    // ���ü�ƽ��
                float cameraFar = camera.GetFarClip();      // Զ�ü�ƽ��

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

            // ����Ⱦ����ͳ��
            ImGui::Begin("Renderer2D Stats");
            {
                auto stats = Renderer2D::GetStats();

                ImGui::Text("FPS: %.3f", fps);      // ֡��

                ImGui::Text("Draw Calls: %d", stats.DrawCalls);
                ImGui::Text("Quad: %d", stats.QuadCount);
                ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
                ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
            }
            ImGui::End();

            // �����ӿ�
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0)); // ���� Gui ������ʽ���߽� = 0
            ImGui::Begin("Scene");
            {
                m_ViewportFocused = ImGui::IsWindowFocused();   // ��ǰ���ڱ��۽�
                m_ViewportHovered = ImGui::IsWindowHovered();   // �����ͣ�ڵ�ǰ����

                Application::GetInstance().GetImGuiLayer()->BlockEvents(/*!m_ViewportFocused ||*/ !m_ViewportHovered); // ��ֹImGui�¼�

                ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();      // ��ǰ����С

                m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };  // �ӿڴ�С

                uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID(); // ��ɫ������ ID

                ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));   // �����ӿ�ͼ��
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