#include "EditorLayer.h"

#include "Lucky/Scene/SceneSerializer.h"

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

        m_SquareObject1 = m_ActiveScene->CreateObject("Square");         // ��������������
        m_SquareObject1.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));    // ��� SpriteRenderer ���

        m_SquareObject2 = m_ActiveScene->CreateObject("Square");         // ��������������
        m_SquareObject2.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));    // ��� SpriteRenderer ���

        m_CameraObject = m_ActiveScene->CreateObject("Main Camera");    // �����������
        m_CameraObject.AddComponent<CameraComponent>();                 // ��� Camera ���
        m_CameraObject.GetComponent<TransformComponent>().Transform.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));

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

        m_ActiveScene->OnUpdate(dt);// ���³���

        m_Framebuffer->Unbind();    // �����֡������
    }

    void EditorLayer::OnImGuiRender()
    {
        static bool dockSpaceOpen = true;
        static bool optFullscreenPersistant = true;
        static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
        bool optFullscreen = optFullscreenPersistant;

        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        if (optFullscreen)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();

            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

            windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        if (dockspaceFlags)
        {
            windowFlags |= ImGuiWindowFlags_NoBackground;
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        // ͣ���ռ� TODO ImGuiLayer
        ImGui::Begin("DockSpace", &dockSpaceOpen, windowFlags);
        {
            ImGui::PopStyleVar();

            if (optFullscreen)
            {
                ImGui::PopStyleVar(2);
            }

            ImGuiIO& io = ImGui::GetIO();

            ImGuiStyle& style = ImGui::GetStyle();      // ��ʽ
            float minWinSizeX = style.WindowMinSize.x;  // ��С���ڴ�С
            style.WindowMinSize.x = 370.0f;

            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspaceID = ImGui::GetID("Editor Dockspace");
                ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockspaceFlags);
            }

            // �˵��� TODO MenuBarPanel
            if (ImGui::BeginMenuBar())
            {
                // �˵���File
                if (ImGui::BeginMenu("File"))
                {
                    // �˵�����س���
                    if (ImGui::MenuItem("Open"))
                    {
                        SceneSerializer serializer(m_ActiveScene);                  // m_ActiveScene �������л���

                        serializer.Deserialize("Assets/Scenes/SampleScene.lucky");  // �����л�
                    }

                    // �˵�����泡��
                    if (ImGui::MenuItem("Save"))
                    {
                        SceneSerializer serializer(m_ActiveScene);                  // m_ActiveScene �������л���

                        serializer.Serialize("Assets/Scenes/SampleScene.lucky");    // ���л�
                    }

                    // �˵���˳�
                    if (ImGui::MenuItem("Exit"))
                    {
                        Application::GetInstance().Close();    // �˳�����
                    }

                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            m_HierarchyPanel.OnImGuiRender();   // ��Ⱦ Hierarchy ���
            m_PropertiesPanel.OnImGuiRender();  // ��Ⱦ Properties ���

            // ����Ⱦ����ͳ�� TODO RendererStatsPanel
            ImGui::Begin("Renderer2D Stats");
            {
                auto stats = Renderer2D::GetStats();

                ImGui::Text("FPS: %.3f", fps);  // ֡��

                ImGui::Text("Draw Calls: %d", stats.DrawCalls);
                ImGui::Text("Quad: %d", stats.QuadCount);
                ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
                ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
            }
            ImGui::End();

            // �����ӿ� TODO SceneViewportPanel
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0)); // ���� Gui ������ʽ���߽� = 0
            ImGui::Begin("Scene");
            {
                m_ViewportFocused = ImGui::IsWindowFocused();   // ��ǰ���ڱ��۽�
                m_ViewportHovered = ImGui::IsWindowHovered();   // �����ͣ�ڵ�ǰ����

                Application::GetInstance().GetImGuiLayer()->BlockEvents(/*!m_ViewportFocused ||*/ !m_ViewportHovered); // ��ֹImGui�¼�

                ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();      // ��ǰ����С

                m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };  // �ӿڴ�С

                uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID(); // ��ɫ������ ID

                ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));   // �����ӿ�ͼ��
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