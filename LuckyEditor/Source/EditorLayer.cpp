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

        // m_EditorCamera = EditorCamera(30.0f, 1280.0f / 720.0f, 0.01f, 1000.0f); // �����༭�����

        m_SquareObject = m_ActiveScene->CreateObject("Square");         // ��������������
        m_SquareObject.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));    // ��� SpriteRenderer ���

        m_CameraObject = m_ActiveScene->CreateObject("Main Camera");    // �����������
        m_CameraObject.AddComponent<CameraComponent>();                 // ��� Camera ���
        m_CameraObject.GetComponent<TransformComponent>().Transform.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));

        m_SceneViewportPanel = SceneViewportPanel(m_Framebuffer, m_ActiveScene);
        m_SceneHierarchyPanel.SetScene(m_ActiveScene);
    }

    void EditorLayer::OnDetach()
    {

    }

    void EditorLayer::OnUpdate(DeltaTime dt)
    {
        m_EditorDockSpace.OnUpdate(dt);

        m_SceneHierarchyPanel.OnUpdate(dt);
        m_InspectorPanel.OnUpdate(dt);
        m_RendererStatsPanel.OnUpdate(dt);
        m_SceneViewportPanel.OnUpdate(dt);
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

        m_SceneHierarchyPanel.OnImGuiRender();       // ��Ⱦ Hierarchy ���
        m_InspectorPanel.OnImGuiRender();       // ��Ⱦ Inspector ���
        m_RendererStatsPanel.OnImGuiRender();   // ��Ⱦ RendererStats ���
        m_SceneViewportPanel.OnImGuiRender();   // ��Ⱦ Viewport ���
    }

    void EditorLayer::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<KeyPressedEvent>(LC_BIND_EVENT_FUNC(EditorLayer::OnKeyPressed));                    // ���ð��������¼�
        dispatcher.Dispatch<MouseButtonPressedEvent>(LC_BIND_EVENT_FUNC(EditorLayer::OnMouseButtonPressed));    // ������갴ť�����¼�

        m_EditorDockSpace.OnEvent(event);
        m_SceneHierarchyPanel.OnEvent(event);
        m_InspectorPanel.OnEvent(event);
        m_RendererStatsPanel.OnEvent(event);
        m_SceneViewportPanel.OnEvent(event);
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

        return false;
    }

    bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        return false;
    }

    void EditorLayer::NewScene()
    {
        m_ActiveScene = CreateRef<Scene>();         // �����³���
        
        m_SceneViewportPanel.SetScene(m_ActiveScene);   // ���� Viewport �ĳ���
        m_SceneHierarchyPanel.SetScene(m_ActiveScene);  // ���� Hierarchy �ĳ���
    }

    void EditorLayer::OpenScene()
    {
        std::optional<std::string> filepath = FileDialogs::OpenFile("Lucky Scene(*.lucky)\0*.lucky\0");    // ���ļ��Ի����ļ�������\0 �ļ�����.lucky��
        
        // ·����Ϊ��
        if (filepath.has_value())
        {
            m_ActiveScene = CreateRef<Scene>();         // �����³���

            m_SceneViewportPanel.SetScene(m_ActiveScene);   // ���� Viewport �ĳ���
            m_SceneHierarchyPanel.SetScene(m_ActiveScene);  // ���� Hierarchy �ĳ���

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