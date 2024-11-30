#include "EditorLayer.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "Lucky/Scene/SceneSerializer.h"
#include "Lucky/Utils/PlatformUtils.h"
#include "Lucky/ImGui/GUI.h"
#include "Lucky/Core/Log.h"

namespace Lucky
{
    EditorLayer::EditorLayer()
        : Layer("EditorLayer")
    {
        m_PlayButtonIcon = Texture2D::Create("Resources/Icons/ToolBar/PlayButton_Icon.png");
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

        m_CurrentScene = CreateRef<Scene>();    // ��������
        m_EditorScene = m_CurrentScene;

        auto commandLineArgs = Application::GetInstance().GetSpecification().CommandLineArgs;
        // �������м��س���
        if (commandLineArgs.Count > 1)
        {
            const char* sceneFilePath = commandLineArgs[1];

            SceneSerializer serializer(m_CurrentScene);
            serializer.Deserialize(sceneFilePath);
        }

        // m_EditorCamera = EditorCamera(30.0f, 1280.0f / 720.0f, 0.01f, 1000.0f); // �����༭�����

        m_CameraObject = m_CurrentScene->CreateObject("Main Camera");    // �����������
        m_CameraObject.AddComponent<CameraComponent>();                 // ��� Camera ���
        m_CameraObject.GetComponent<TransformComponent>().Transform.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));

        m_SceneHierarchyPanel = CreateRef<SceneHierarchyPanel>(m_CurrentScene);
        m_InspectorPanel = CreateRef<InspectorPanel>();
        m_SceneViewportPanel = CreateRef<SceneViewportPanel>(m_SceneViewportFramebuffer, m_CurrentScene);
        m_GameViewportPanel = CreateRef<GameViewportPanel>(m_GameViewportFramebuffer, m_CurrentScene);
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

        switch (m_SceneState)
        {
            case Lucky::EditorLayer::SceneState::Edit:
                m_GameViewportPanel->OnUpdateEditor(dt);    // Temp �������������Ⱦ�ı༭������
                break;
            case Lucky::EditorLayer::SceneState::Play:
                // TODO ��������ͽű���
                m_GameViewportPanel->OnUpdateRuntime(dt);   // Temp ��������
                break;
            default:
                break;
        }
    }

    void EditorLayer::OnImGuiRender()
    {
        // ���� DockSpace
        m_EditorDockSpace.Setup([&]()
        {
            UI_ToolBar();   // ��Ⱦ ToolBar
        });

        UI_MenuBar();       // ��Ⱦ MenuBar

        static bool isOpen = true;
        m_SceneHierarchyPanel->OnImGuiRender(isOpen);   // ��Ⱦ Hierarchy ���
        m_InspectorPanel->OnImGuiRender(isOpen);        // ��Ⱦ Inspector ���
        m_GameViewportPanel->OnImGuiRender(isOpen);     // ��Ⱦ Game Viewport ���
        m_SceneViewportPanel->OnImGuiRender(isOpen);    // ��Ⱦ Scene Viewport ���
        m_RendererStatsPanel->OnImGuiRender(isOpen);    // ��Ⱦ RendererStats ���
        m_ProjectAssetsPanel->OnImGuiRender(isOpen);    // ��Ⱦ ProjectAssets ���

        //ImGui::ShowDemoWindow();
    }

    void EditorLayer::UI_MenuBar()
    {
        if (ImGui::BeginMainMenuBar())
        {
            m_MenuBarHeight = ImGui::GetCurrentWindow()->Size.y;

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

                // ���泡��
                if (ImGui::MenuItem("Save", "Ctrl+S"))
                {
                    SaveScene();
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
    }

    void EditorLayer::UI_ToolBar()
    {
        float panelWidth = ImGui::GetWindowContentRegionWidth();    // �����
        float lineHeight = GImGui->Font->FontSize;
        
        ImGui::Dummy({ 0, m_MenuBarHeight });
        ImGui::SameLine();

        static ImVec4 playButtonColor = { 0.2196f, 0.2196f, 0.2196f, 1.0f };    // ԭɫ

        static glm::vec4 playButtonColors[4] =
        {
            { 0.2196f, 0.2196f, 0.2196f, 1.0f },    // ԭɫ
            { 0.3882f, 0.3882f, 0.3882f, 1.0f },    // ��ͣ
            { 0.2196f, 0.2196f, 0.2196f, 1.0f },    // ����
            { 0.2588f, 0.5882f, 0.9804f, 0.3490f }  // ѡ��
        };

        // ���ð�ť��λ��
        ImGui::SetCursorPos({ (panelWidth - lineHeight) * 0.5f, ImGui::GetCursorPosY() });

        GUI::SelectableImageButton(m_PlayButtonIcon->GetRendererID(), { lineHeight * 0.8f, lineHeight * 0.8f }, playButtonColors, [&]
        {
            switch (m_SceneState)
            {
                case SceneState::Edit:
                    OnScenePlay();  // ����
                    break;
                case SceneState::Play:
                    OnSceneStop();
                    break;
            }
        });
    }

    void EditorLayer::OnScenePlay()
    {
        m_SceneState = SceneState::Play;

        m_RuntimeScene = Scene::Copy(m_EditorScene); // ���Ʊ༭״̬�ĳ���

        // �����������ĳ���������
        m_SceneViewportPanel->SetSceneContext(m_RuntimeScene);
        m_GameViewportPanel->SetSceneContext(m_RuntimeScene);
        m_SceneHierarchyPanel->SetSceneContext(m_RuntimeScene);

        m_RuntimeScene->OnRuntimeStart();   // ��ʼ����

        ImGui::SetWindowFocus("Game");  // TODO Temp �۽� Game ����

        m_CurrentScene = m_RuntimeScene;
    }

    void EditorLayer::OnSceneStop()
    {
        m_RuntimeScene->OnRuntimeStop(); // ֹͣ����

        m_SceneState = SceneState::Edit;

        m_RuntimeScene = nullptr;       // ж������ʱ����

        // �����������ĳ���������
        m_SceneViewportPanel->SetSceneContext(m_EditorScene);
        m_GameViewportPanel->SetSceneContext(m_EditorScene);
        m_SceneHierarchyPanel->SetSceneContext(m_EditorScene);

        ImGui::SetWindowFocus("Scene"); // Temp TODO �۽�����ǰ�Ľ��㴰�ڣ��ָ��༭�����ݣ�

        m_CurrentScene = m_EditorScene; // �ָ���ǰ�������༭ʱ����
    }

    void EditorLayer::NewScene()
    {
        m_EditorScene = CreateRef<Scene>();         // �����³���
        m_SceneFilePath = std::filesystem::path();  // ����·��

        // �����������ĳ���������
        m_SceneViewportPanel->SetSceneContext(m_EditorScene);
        m_GameViewportPanel->SetSceneContext(m_EditorScene);
        m_SceneHierarchyPanel->SetSceneContext(m_EditorScene);

        m_CurrentScene = m_EditorScene;
    }

    void EditorLayer::OpenScene()
    {
        // ���ļ��Ի����ļ�������\0 �ļ�����.lucky��
        std::string filepath = FileDialogs::OpenFile("Lucky Scene(*.lucky)\0*.lucky\0");

        // ·����Ϊ��
        if (!filepath.empty())
        {
            OpenScene(filepath);    // �򿪳���
        }
    }

    void EditorLayer::OpenScene(const std::filesystem::path& filepath)
    {
        // �Ǳ༭״̬
        if (m_SceneState != SceneState::Edit)
        {
            OnSceneStop();  // ֹͣ���г���
        }

        // ���ǳ����ļ�
        if (filepath.extension().string() != ".lucky")
        {
            LC_WARN("Can not Load {0} - Not a Scene File.", filepath.filename().string());
            return;
        }

        Ref<Scene> newScene = CreateRef<Scene>();   // �����³���
        SceneSerializer serializer(newScene);       // �������л���
        // �����л��������ļ��������³���
        if (serializer.Deserialize(filepath.string()))
        {
            m_EditorScene = newScene;

            // �����������ĳ���������
            m_SceneViewportPanel->SetSceneContext(m_EditorScene);
            m_GameViewportPanel->SetSceneContext(m_EditorScene);
            m_SceneHierarchyPanel->SetSceneContext(m_EditorScene);

            m_CurrentScene = m_EditorScene; // ��ǰ����
            m_SceneFilePath = filepath;     // ��ǰ����·��
        }
    }

    void EditorLayer::SaveScene()
    {
        if (!m_SceneFilePath.empty())
        {
            SerializeScene(m_EditorScene, m_SceneFilePath); // ���л����������ǰ����
        }
        else
        {
            SaveSceneAs();  // �������Ϊ
        }
    }

    void EditorLayer::SaveSceneAs()
    {
        std::string filepath = FileDialogs::SaveFile("Lucky Scene(*.lucky)\0*.lucky\0");    // �����ļ��Ի����ļ�������\0 �ļ�����.lucky��

        // ·����Ϊ��
        if (!filepath.empty())
        {
            SerializeScene(m_EditorScene, filepath);    // ���л�����
            m_SceneFilePath = filepath;             // ����·��
        }
    }

    void EditorLayer::SerializeScene(Ref<Scene> scene, const std::filesystem::path& filepath)
    {
        SceneSerializer serializer(scene);          // �������л���
        serializer.Serialize(filepath.string());    // ���л������泡��
    }

    void EditorLayer::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<KeyPressedEvent>(LC_BIND_EVENT_FUNC(EditorLayer::OnKeyPressed));    // ���ð��������¼�

        m_SceneHierarchyPanel->OnEvent(event);
        m_SceneViewportPanel->OnEvent(event);
    }

    bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
    {
        // �����ظ�
        if (e.IsRepeat())
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
            if (control)
            {
                if (shift)
                {
                    SaveSceneAs();  // �������Ϊ��Ctrl+Shift+S
                }
                else
                {
                    SaveScene();    // ���浽��ǰ������ Ctrl+S
                }
            }
            break;
        }

        return false;
    }
}