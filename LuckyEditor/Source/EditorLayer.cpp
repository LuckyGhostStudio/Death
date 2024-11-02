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
        FramebufferSpecification fbSpec; // 帧缓冲区规范

        fbSpec.Attachments =
        {
            FramebufferTextureFormat::RGBA8,        // 颜色缓冲区 0
            FramebufferTextureFormat::RED_INTEGER,  // 颜色缓冲区 1：作为 id 实现鼠标点击拾取
            FramebufferTextureFormat::Depth         // 深度缓冲区
        };

        fbSpec.Width = 1280;
        fbSpec.Height = 720;

        m_Framebuffer = Framebuffer::Create(fbSpec);    // 创建帧缓冲区

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
        m_EditorDockSpace.OnImGuiRender();    // 渲染 DockSpace

        // 菜单条 TODO MenuBarPanel
        if (ImGui::BeginMainMenuBar())
        {
            // 菜单：File
            if (ImGui::BeginMenu("File"))
            {
                // 创建新场景
                if (ImGui::MenuItem("New", "Ctrl N"))
                {
                    NewScene();
                }

                // 打开文件：加载场景
                if (ImGui::MenuItem("Open...", "Ctrl O"))
                {
                    OpenScene();
                }

                // 另存为：保存场景
                if (ImGui::MenuItem("Save As...", "Shift Ctrl S"))
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

        m_SceneHierarchyPanel.OnImGuiRender();       // 渲染 Hierarchy 面板
        m_InspectorPanel.OnImGuiRender();       // 渲染 Inspector 面板
        m_RendererStatsPanel.OnImGuiRender();   // 渲染 RendererStats 面板
        m_SceneViewportPanel.OnImGuiRender();   // 渲染 Viewport 面板
    }

    void EditorLayer::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<KeyPressedEvent>(LC_BIND_EVENT_FUNC(EditorLayer::OnKeyPressed));                    // 调用按键按下事件
        dispatcher.Dispatch<MouseButtonPressedEvent>(LC_BIND_EVENT_FUNC(EditorLayer::OnMouseButtonPressed));    // 调用鼠标按钮按下事件

        m_EditorDockSpace.OnEvent(event);
        m_SceneHierarchyPanel.OnEvent(event);
        m_InspectorPanel.OnEvent(event);
        m_RendererStatsPanel.OnEvent(event);
        m_SceneViewportPanel.OnEvent(event);
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
                    NewScene();     // 创建新场景：Ctrl N
                }
                break;
            case Key::O:
                if (control)
                {
                    OpenScene();    // 打开场景：Ctrl O
                }
                break;
            case Key::S:
                if (control && shift)
                {
                    SaveSceneAs();  // 场景另存为：Shift Ctrl S
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
        
        m_SceneViewportPanel.SetScene(m_ActiveScene);   // 设置 Viewport 的场景
        m_SceneHierarchyPanel.SetScene(m_ActiveScene);  // 设置 Hierarchy 的场景
    }

    void EditorLayer::OpenScene()
    {
        std::optional<std::string> filepath = FileDialogs::OpenFile("Lucky Scene(*.lucky)\0*.lucky\0");    // 打开文件对话框（文件类型名\0 文件类型.lucky）
        
        // 路径不为空
        if (filepath.has_value())
        {
            m_ActiveScene = CreateRef<Scene>();         // 创建新场景

            m_SceneViewportPanel.SetScene(m_ActiveScene);   // 设置 Viewport 的场景
            m_SceneHierarchyPanel.SetScene(m_ActiveScene);  // 设置 Hierarchy 的场景

            SceneSerializer serializer(m_ActiveScene);  // 场景序列化器
            serializer.Deserialize(filepath.value());   // 反序列化：加载文件场景到新场景
        }
    }

    void EditorLayer::SaveSceneAs()
    {
        std::optional<std::string> filepath = FileDialogs::SaveFile("Lucky Scene(*.lucky)\0*.lucky\0");    // 保存文件对话框（文件类型名\0 文件类型.lucky）

        // 路径不为空
        if (filepath.has_value())
        {
            SceneSerializer serializer(m_ActiveScene);  // 场景序列化器
            serializer.Serialize(filepath.value());     // 序列化：保存场景
        }
    }
}