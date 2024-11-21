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

        m_SceneViewportFramebuffer = Framebuffer::Create(fbSpec);   // ���� Scene Viewport ֡������
        m_GameViewportFramebuffer = Framebuffer::Create(fbSpec);    // ���� Game Viewport ֡������

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

        m_SceneHierarchyPanel = CreateRef<SceneHierarchyPanel>(m_ActiveScene);
        m_InspectorPanel = CreateRef<InspectorPanel>();
        m_SceneViewportPanel = CreateRef<SceneViewportPanel>(m_SceneViewportFramebuffer, m_ActiveScene);
        m_GameViewportPanel = CreateRef<GameViewportPanel>(m_GameViewportFramebuffer, m_ActiveScene);
        m_RendererStatsPanel = CreateRef<RendererStatsPanel>();
        m_ProjectAssetsPanel = CreateRef<ProjectAssetsPanel>();
    }

    void EditorLayer::OnDetach()
    {

    }

    void EditorLayer::OnUpdate(DeltaTime dt)
    {
        m_RendererStatsPanel->OnUpdate(dt);
        m_SceneViewportPanel->OnUpdate(dt);
        m_GameViewportPanel->OnUpdate(dt);
    }

    void EditorLayer::OnImGuiRender()
    {
        m_EditorDockSpace.Setup();  // ���� DockSpace

        // �˵��� TODO MenuBarPanel
        if (ImGui::BeginMainMenuBar())
        {
            // �˵���File
            if (ImGui::BeginMenu("File"))
            {
                // �����³���
                if (ImGui::MenuItem("New", "Ctrl+N"))
                {
                    NewScene();
                }

                // ���ļ������س���
                if (ImGui::MenuItem("Open...", "Ctrl+O"))
                {
                    OpenScene();
                }

                // ���Ϊ�����泡��
                if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
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

        static bool isOpen = true;
        m_SceneHierarchyPanel->OnImGuiRender(isOpen);   // ��Ⱦ Hierarchy ���
        m_InspectorPanel->OnImGuiRender(isOpen);        // ��Ⱦ Inspector ���
        m_SceneViewportPanel->OnImGuiRender(isOpen);    // ��Ⱦ Scene Viewport ���
        m_GameViewportPanel->OnImGuiRender(isOpen);     // ��Ⱦ Game Viewport ���
        m_RendererStatsPanel->OnImGuiRender(isOpen);    // ��Ⱦ RendererStats ���
        m_ProjectAssetsPanel->OnImGuiRender(isOpen);    // ��Ⱦ ProjectAssets ���
    }

    void EditorLayer::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<KeyPressedEvent>(LC_BIND_EVENT_FUNC(EditorLayer::OnKeyPressed));                    // ���ð��������¼�
        dispatcher.Dispatch<MouseButtonPressedEvent>(LC_BIND_EVENT_FUNC(EditorLayer::OnMouseButtonPressed));    // ������갴ť�����¼�

        m_SceneHierarchyPanel->OnEvent(event);
        m_SceneViewportPanel->OnEvent(event);
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
                    NewScene();     // �����³�����Ctrl+N
                }
                break;
            case Key::O:
                if (control)
                {
                    OpenScene();    // �򿪳�����Ctrl+O
                }
                break;
            case Key::S:
                if (control && shift)
                {
                    SaveSceneAs();  // �������Ϊ��Ctrl+Shift+S
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
        
        m_SceneViewportPanel->SetSceneContext(m_ActiveScene);   // ���� Scene Viewport �ĳ���
        m_GameViewportPanel->SetSceneContext(m_ActiveScene);    // ���� Game Viewport �ĳ���
        m_SceneHierarchyPanel->SetSceneContext(m_ActiveScene);  // ���� Hierarchy �ĳ���
    }

    void EditorLayer::OpenScene()
    {
        std::string filepath = FileDialogs::OpenFile("Lucky Scene(*.lucky)\0*.lucky\0");    // ���ļ��Ի����ļ�������\0 �ļ�����.lucky��

        // ·����Ϊ��
        if (!filepath.empty())
        {
            OpenScene(filepath);    // �򿪳���
        }
    }

    void EditorLayer::OpenScene(const std::filesystem::path& filepath)
    {
        m_ActiveScene = CreateRef<Scene>();             // �����³���

        m_SceneViewportPanel->SetSceneContext(m_ActiveScene);   // ���� Scene Viewport �ĳ���
        m_GameViewportPanel->SetSceneContext(m_ActiveScene);    // ���� Game Viewport �ĳ���
        m_SceneHierarchyPanel->SetSceneContext(m_ActiveScene);  // ���� Hierarchy �ĳ���

        SceneSerializer serializer(m_ActiveScene);      // �������л���
        serializer.Deserialize(filepath.string());      // �����л��������ļ��������³���
    }

    void EditorLayer::SaveSceneAs()
    {
        std::string filepath = FileDialogs::SaveFile("Lucky Scene(*.lucky)\0*.lucky\0");    // �����ļ��Ի����ļ�������\0 �ļ�����.lucky��

        // ·����Ϊ��
        if (!filepath.empty())
        {
            SceneSerializer serializer(m_ActiveScene);  // �������л���
            serializer.Serialize(filepath);             // ���л������泡��
        }
    }
}