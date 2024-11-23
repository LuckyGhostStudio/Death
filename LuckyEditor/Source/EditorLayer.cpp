#include "EditorLayer.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "Lucky/Scene/SceneSerializer.h"
#include "Lucky/Utils/PlatformUtils.h"
#include "Lucky/ImGui/GUI.h"

namespace Lucky
{
    EditorLayer::EditorLayer()
        : Layer("EditorLayer")
    {
        m_PlayButtonIcon = Texture2D::Create("Resources/Icons/ToolBar/PlayButton_Icon.png");
    }

    void EditorLayer::OnAttach()
    {
        FramebufferSpecification fbSpec; // 帧缓冲区规范

        fbSpec.Attachments =
        {
            FramebufferTextureFormat::RGBA8,        // 颜色缓冲区 0
            FramebufferTextureFormat::RED_INTEGER,  // 颜色缓冲区 1：作为 id 实现鼠标点击拾取
            FramebufferTextureFormat::Depth         // 深度缓冲区
        };

        fbSpec.Width = 1280;
        fbSpec.Height = 720;

        m_SceneViewportFramebuffer = Framebuffer::Create(fbSpec);   // 创建 Scene Viewport 帧缓冲区
        m_GameViewportFramebuffer = Framebuffer::Create(fbSpec);    // 创建 Game Viewport 帧缓冲区

        m_ActiveScene = CreateRef<Scene>();             // 创建场景

        auto commandLineArgs = Application::GetInstance().GetCommandLineArgs();
        // 从命令行加载场景
        if (commandLineArgs.Count > 1)
        {
            const char* sceneFilePath = commandLineArgs[1];

            SceneSerializer serializer(m_ActiveScene);
            serializer.Deserialize(sceneFilePath);
        }

        // m_EditorCamera = EditorCamera(30.0f, 1280.0f / 720.0f, 0.01f, 1000.0f); // 创建编辑器相机

        m_SquareObject = m_ActiveScene->CreateObject("Square");         // 创建正方形物体
        m_SquareObject.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));    // 添加 SpriteRenderer 组件

        m_CameraObject = m_ActiveScene->CreateObject("Main Camera");    // 创建相机对象
        m_CameraObject.AddComponent<CameraComponent>();                 // 添加 Camera 组件
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
        switch (m_SceneState)
        {
            case Lucky::EditorLayer::SceneState::Edit:

                break;
            case Lucky::EditorLayer::SceneState::Play:
                // TODO 更新物理和脚本等
                break;
            default:
                break;
        }

        m_RendererStatsPanel->OnUpdate(dt);
        m_SceneViewportPanel->OnUpdate(dt);
        m_GameViewportPanel->OnUpdate(dt);
    }

    void EditorLayer::OnImGuiRender()
    {
        // 设置 DockSpace
        m_EditorDockSpace.Setup([&]()
        {
            UI_ToolBar();   // 渲染 ToolBar
        });

        UI_MenuBar();       // 渲染 MenuBar

        static bool isOpen = true;
        m_SceneHierarchyPanel->OnImGuiRender(isOpen);   // 渲染 Hierarchy 面板
        m_InspectorPanel->OnImGuiRender(isOpen);        // 渲染 Inspector 面板
        m_GameViewportPanel->OnImGuiRender(isOpen);     // 渲染 Game Viewport 面板
        m_SceneViewportPanel->OnImGuiRender(isOpen);    // 渲染 Scene Viewport 面板
        m_RendererStatsPanel->OnImGuiRender(isOpen);    // 渲染 RendererStats 面板
        m_ProjectAssetsPanel->OnImGuiRender(isOpen);    // 渲染 ProjectAssets 面板

        //ImGui::ShowDemoWindow();
    }

    void EditorLayer::UI_MenuBar()
    {
        if (ImGui::BeginMainMenuBar())
        {
            m_MenuBarHeight = ImGui::GetCurrentWindow()->Size.y;

            // 菜单：File
            if (ImGui::BeginMenu("File"))
            {
                // 创建新场景
                if (ImGui::MenuItem("New", "Ctrl+N"))
                {
                    NewScene();
                }

                // 打开文件：加载场景
                if (ImGui::MenuItem("Open...", "Ctrl+O"))
                {
                    OpenScene();
                }

                // 另存为：保存场景
                if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
                {
                    SaveSceneAs();
                }

                // 菜单项：退出
                if (ImGui::MenuItem("Quit"))
                {
                    Application::GetInstance().Close();    // 退出程序
                }

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
    }

    void EditorLayer::UI_ToolBar()
    {
        float panelWidth = ImGui::GetWindowContentRegionWidth();    // 面板宽度
        float lineHeight = GImGui->Font->FontSize;
        
        ImGui::Dummy({ 0, m_MenuBarHeight });
        ImGui::SameLine();

        static ImVec4 playButtonColor = { 0.2196f, 0.2196f, 0.2196f, 1.0f };    // 原色

        static glm::vec4 playButtonColors[4] =
        {
            { 0.2196f, 0.2196f, 0.2196f, 1.0f },    // 原色
            { 0.3882f, 0.3882f, 0.3882f, 1.0f },    // 悬停
            { 0.2196f, 0.2196f, 0.2196f, 1.0f },    // 激活
            { 0.2588f, 0.5882f, 0.9804f, 0.3490f }  // 选中
        };

        // 设置按钮的位置
        ImGui::SetCursorPos({ (panelWidth - lineHeight) * 0.5f, ImGui::GetCursorPosY() });

        GUI::SelectableImageButton(m_PlayButtonIcon->GetRendererID(), { lineHeight * 0.8f, lineHeight * 0.8f }, playButtonColors, [&]
        {
            switch (m_SceneState)
            {
                case SceneState::Edit:
                    OnScenePlay();  // 运行
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

        ImGui::SetWindowFocus("Game");  // 聚焦 Game 窗口
    }

    void EditorLayer::OnSceneStop()
    {
        m_SceneState = SceneState::Edit;

        // temp TODO 聚焦激活的窗口（恢复编辑器数据）
        ImGui::SetWindowFocus("Scene");
    }

    void EditorLayer::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<KeyPressedEvent>(LC_BIND_EVENT_FUNC(EditorLayer::OnKeyPressed));                    // 调用按键按下事件
        dispatcher.Dispatch<MouseButtonPressedEvent>(LC_BIND_EVENT_FUNC(EditorLayer::OnMouseButtonPressed));    // 调用鼠标按钮按下事件

        m_SceneHierarchyPanel->OnEvent(event);
        m_SceneViewportPanel->OnEvent(event);
    }

    bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
    {
        // 快捷键：重复次数 == 0
        if (e.GetRepeatCount() > 0)
        {
            return false;
        }

        bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl); // Ctrl 键按下
        bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);       // Shift 键按下

        // 菜单快捷键
        switch (e.GetKeyCode())
        {
            case Key::N:
                if (control)
                {
                    NewScene();     // 创建新场景：Ctrl+N
                }
                break;
            case Key::O:
                if (control)
                {
                    OpenScene();    // 打开场景：Ctrl+O
                }
                break;
            case Key::S:
                if (control && shift)
                {
                    SaveSceneAs();  // 场景另存为：Ctrl+Shift+S
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
        m_ActiveScene = CreateRef<Scene>();         // 创建新场景
        
        m_SceneViewportPanel->SetSceneContext(m_ActiveScene);   // 设置 Scene Viewport 的场景
        m_GameViewportPanel->SetSceneContext(m_ActiveScene);    // 设置 Game Viewport 的场景
        m_SceneHierarchyPanel->SetSceneContext(m_ActiveScene);  // 设置 Hierarchy 的场景
    }

    void EditorLayer::OpenScene()
    {
        std::string filepath = FileDialogs::OpenFile("Lucky Scene(*.lucky)\0*.lucky\0");    // 打开文件对话框（文件类型名\0 文件类型.lucky）

        // 路径不为空
        if (!filepath.empty())
        {
            OpenScene(filepath);    // 打开场景
        }
    }

    void EditorLayer::OpenScene(const std::filesystem::path& filepath)
    {
        m_ActiveScene = CreateRef<Scene>();             // 创建新场景

        m_SceneViewportPanel->SetSceneContext(m_ActiveScene);   // 设置 Scene Viewport 的场景
        m_GameViewportPanel->SetSceneContext(m_ActiveScene);    // 设置 Game Viewport 的场景
        m_SceneHierarchyPanel->SetSceneContext(m_ActiveScene);  // 设置 Hierarchy 的场景

        SceneSerializer serializer(m_ActiveScene);      // 场景序列化器
        serializer.Deserialize(filepath.string());      // 反序列化：加载文件场景到新场景
    }

    void EditorLayer::SaveSceneAs()
    {
        std::string filepath = FileDialogs::SaveFile("Lucky Scene(*.lucky)\0*.lucky\0");    // 保存文件对话框（文件类型名\0 文件类型.lucky）

        // 路径不为空
        if (!filepath.empty())
        {
            SceneSerializer serializer(m_ActiveScene);  // 场景序列化器
            serializer.Serialize(filepath);             // 序列化：保存场景
        }
    }
}