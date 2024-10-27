#include "EditorLayer.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "Lucky/Scene/SceneSerializer.h"
#include "Lucky/Utils/PlatformUtils.h"

#include "ImGuizmo.h"
#include "Lucky/Math/Math.h"

namespace Lucky
{
    EditorLayer::EditorLayer()
        : Layer("EditorLayer")
    {

    }

    void EditorLayer::OnAttach()
    {
        FramebufferSpecification fbSpec; // ֡�������淶

        fbSpec.Attachments =
        {
            FramebufferTextureFormat::RGBA8,        // ��ɫ������ 0
            FramebufferTextureFormat::RED_INTEGER,  // ��ɫ������ 1����Ϊ id ʵ�������ʰȡ
            FramebufferTextureFormat::Depth         // ��Ȼ�����
        };

        fbSpec.Width = 1280;
        fbSpec.Height = 720;

        m_Framebuffer = Framebuffer::Create(fbSpec);    // ����֡������

        m_ActiveScene = CreateRef<Scene>();             // ��������

        auto commandLineArgs = Application::GetInstance().GetCommandLineArgs();
        // �������м��س���
        if (commandLineArgs.Count > 1)
        {
            const char* sceneFilePath = commandLineArgs[1];

            SceneSerializer serializer(m_ActiveScene);
            serializer.Deserialize(sceneFilePath);
        }

        m_EditorCamera = EditorCamera(30.0f, 1280.0f / 720.0f, 0.01f, 1000.0f); // �����༭�����

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
        if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);  // ����֡��������С

            m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);                         // �����ӿڴ�С
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);    // �����ӿڴ�С
        }

        // �ӿڱ��۽�
        if (m_ViewportFocused)
        {

        }

        m_EditorCamera.OnUpdate(dt);    // ���±༭�����

        Renderer2D::ResetStats();   // ����ͳ������

        m_Framebuffer->Bind();      // ��֡������

        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        RenderCommand::Clear();

        m_Framebuffer->ClearAttachment(1, -1);  // �����ɫ������ 1������ id ��������Ϊ -1

        m_ActiveScene->OnUpdateEditor(dt, m_EditorCamera);  // ���±༭������

        auto [mx, my] = ImGui::GetMousePos();   // ���λ��

        // �������������ӿ����Ͻǵ�λ��
        mx -= m_ViewportBounds[0].x;
        my -= m_ViewportBounds[0].y;

        glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0]; // �ӿڴ�С������ tab bar

        my = viewportSize.y - my;   // ��ת y ���� ���½�0,0�������������½�Ϊ0,0��

        // ���������ӿ����Ͻǵ�λ��
        int mouseX = (int)mx;
        int mouseY = (int)my;
        
        // ������ӿ���
        if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
        {
            int pixelData = m_Framebuffer->GetPixel(1, mouseX, mouseY); // ��ȡ 1 ����ɫ����������
            // ����ʰȡ�����壨���� -1��
            m_PickedObject = pixelData == -1 ? Object() : Object((entt::entity)pixelData, m_ActiveScene.get());

            LC_CORE_WARN("pixelData:{0}", pixelData);
        }

        m_Framebuffer->Unbind();    // �����֡������
    }

    void EditorLayer::OnImGuiRender()
    {
        m_EditorDockSpace.OnImGuiRender();    // ��Ⱦ DockSpace

        // �˵��� TODO MenuBarPanel
        if (ImGui::BeginMainMenuBar())
        {
            // �˵���File
            if (ImGui::BeginMenu("File"))
            {
                // �����³���
                if (ImGui::MenuItem("New", "Ctrl N"))
                {
                    NewScene();
                }

                // ���ļ������س���
                if (ImGui::MenuItem("Open...", "Ctrl O"))
                {
                    OpenScene();
                }

                // ���Ϊ�����泡��
                if (ImGui::MenuItem("Save As...", "Shift Ctrl S"))
                {
                    SaveSceneAs();
                }

                // �˵���˳�
                if (ImGui::MenuItem("Quit"))
                {
                    Application::GetInstance().Close();    // �˳�����
                }

                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        m_HierarchyPanel.OnImGuiRender();   // ��Ⱦ Hierarchy ���
        m_PropertiesPanel.OnImGuiRender();  // ��Ⱦ Properties ���

        // ����Ⱦ����ͳ�� TODO RendererStatsPanel
        ImGui::Begin("Scece Renderer Stats");
        {
            auto stats = Renderer2D::GetStats();

            ImGui::Text("FPS: %.3f", Application::GetInstance().GetFPS());  // ֡��

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
            auto viewportMinRegion = ImGui::GetWindowContentRegionMin();    // �ӿڿ���������Сֵ���ӿ����Ͻ�������ӿ����Ͻ�λ�ã�
            auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();    // �ӿڿ����������ֵ���ӿ����½�������ӿ����Ͻ�λ�ã�
            auto viewportOffset = ImGui::GetWindowPos();                    // �ӿ�ƫ�������ӿ�������Ͻ�λ�ã��������Ļ���Ͻǣ�

            m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
            m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

            m_ViewportFocused = ImGui::IsWindowFocused();   // ��ǰ���ڱ��۽�
            m_ViewportHovered = ImGui::IsWindowHovered();   // �����ͣ�ڵ�ǰ����

            Application::GetInstance().GetImGuiLayer()->BlockEvents(/*!m_ViewportFocused ||*/ !m_ViewportHovered); // ��ֹImGui�¼�

            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();      // ��ǰ����С

            m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };  // �ӿڴ�С

            uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID(); // ��ɫ������ 0 ID

            ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));   // �����ӿ�ͼ��

            // Gizmo
            Object selectedObject = Selection::Object;  // ��ѡ������
            // ѡ��������� && Gizmo ���ʹ���
            if (selectedObject && m_GizmoType != -1)
            {
                ImGuizmo::SetOrthographic(false);   // ͸��ͶӰ
                ImGuizmo::SetDrawlist();            // ���û����б�

                // ���û�������
                ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

                // �༭�����
                const glm::mat4& cameraProjection = m_EditorCamera.GetProjectionMatrix();   // ͶӰ����
                glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();                      // ��ͼ����

                // ��������ϵ
                //ImGuizmo::DrawGrid(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), glm::value_ptr(glm::mat4(1.0f)), 10);

                // ��ѡ������ transform
                Transform& transformComponent = selectedObject.GetComponent<TransformComponent>().Transform;    // Transform
                glm::mat4 transform = transformComponent.GetTransform();

                bool span = Input::IsKeyPressed(Key::LeftControl);  // Ctrl �̶Ȳ�׽������ʱ�̶� delta �̶�
                float spanValue = 0.5f;     // ƽ�����ż����0.5m

                // ��ת���ֵ��5 ��
                if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
                {
                    spanValue = 5.0f;
                }
                    
                float spanValues[3] = { spanValue, spanValue, spanValue };  // xyz ��̶Ȳ�׽ֵ

                // ���Ʋ��� Gizmo�������ͼ���� ���ͶӰ���� Gizmo ���� ��������ϵ ѡ������ transform �������� �̶Ȳ�׽ֵ
                ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                    (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
                    nullptr, span ? spanValues : nullptr);

                // Gizmo ����ʹ��
                if (ImGuizmo::IsUsing())
                {
                    glm::vec3 position, rotation, scale;
                    Math::DecomposeTransform(transform, position, rotation, scale); // �ֽ� transform ����
                        
                    switch (m_GizmoType)
                    {
                        case ImGuizmo::OPERATION::TRANSLATE:
                            transformComponent.GetPosition() = position;        // ����λ��
                            break;
                        case ImGuizmo::OPERATION::ROTATE:
                            glm::vec3 deltaRotation = rotation - transformComponent.GetRotation();  // ��ת����
                            transformComponent.GetRotation() += deltaRotation;  // ������ת���ۼ������������������
                            break;
                        case ImGuizmo::OPERATION::SCALE:
                            transformComponent.GetScale() = scale;              // ��������
                            break;
                    }
                }
            }
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }

    void EditorLayer::OnEvent(Event& event)
    {
        m_EditorCamera.OnEvent(event);  // �༭������¼�

        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<KeyPressedEvent>(LC_BIND_EVENT_FUNC(EditorLayer::OnKeyPressed));                    // ���ð��������¼�
        dispatcher.Dispatch<MouseButtonPressedEvent>(LC_BIND_EVENT_FUNC(EditorLayer::OnMouseButtonPressed));    // ������갴ť�����¼�
    }

    bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
    {
        // ��ݼ����ظ����� == 0
        if (e.GetRepeatCount() > 0)
        {
            return false;
        }

        bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl); // Ctrl ������
        bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);       // Shift ������

        // �˵���ݼ�
        switch (e.GetKeyCode())
        {
            case Key::N:
                if (control)
                {
                    NewScene();     // �����³�����Ctrl N
                }
                break;
            case Key::O:
                if (control)
                {
                    OpenScene();    // �򿪳�����Ctrl O
                }
                break;
            case Key::S:
                if (control && shift)
                {
                    SaveSceneAs();  // �������Ϊ��Shift Ctrl S
                }
                break;
        }

        // Gizmo ��������ʹ��
        if (!ImGuizmo::IsUsing())
        {
            // Gizmo ��ݼ�
            switch (e.GetKeyCode())
            {
                case Key::G:
                    m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;   // ƽ��
                    break;
                case Key::R:
                    m_GizmoType = ImGuizmo::OPERATION::ROTATE;      // ��ת
                    break;
                case Key::S:
                    m_GizmoType = ImGuizmo::OPERATION::SCALE;       // ����
                    break;
            }
        }

        return false;
    }

    bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        // ����������
        if (e.GetMouseButton() == Mouse::ButtonLeft)
        {
            // ������ӿ��� Gizmo ����δ����
            if (m_ViewportHovered && !ImGuizmo::IsOver())
            {
                Selection::Object = m_PickedObject; // ���ñ�ѡ������
            }
        }

        return false;
    }

    void EditorLayer::NewScene()
    {
        m_ActiveScene = CreateRef<Scene>();         // �����³���
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);    // �����ӿڴ�С
        m_HierarchyPanel.SetScene(m_ActiveScene);   // ���� Hierarchy �ĳ���
    }

    void EditorLayer::OpenScene()
    {
        std::optional<std::string> filepath = FileDialogs::OpenFile("Lucky Scene(*.lucky)\0*.lucky\0");    // ���ļ��Ի����ļ�������\0 �ļ�����.lucky��
        
        // ·����Ϊ��
        if (filepath.has_value())
        {
            m_ActiveScene = CreateRef<Scene>();         // �����³���
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);    // �����ӿڴ�С
            m_HierarchyPanel.SetScene(m_ActiveScene);   // ���� Hierarchy �ĳ���

            SceneSerializer serializer(m_ActiveScene);  // �������л���
            serializer.Deserialize(filepath.value());   // �����л��������ļ��������³���
        }
    }

    void EditorLayer::SaveSceneAs()
    {
        std::optional<std::string> filepath = FileDialogs::SaveFile("Lucky Scene(*.lucky)\0*.lucky\0");    // �����ļ��Ի����ļ�������\0 �ļ�����.lucky��

        // ·����Ϊ��
        if (filepath.has_value())
        {
            SceneSerializer serializer(m_ActiveScene);  // �������л���
            serializer.Serialize(filepath.value());     // ���л������泡��
        }
    }
}