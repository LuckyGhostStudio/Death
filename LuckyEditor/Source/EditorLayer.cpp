#include "EditorLayer.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>

#include "Lucky/Scene/SceneSerializer.h"
#include "Lucky/Utils/PlatformUtils.h"


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

        m_SquareObject1 = m_ActiveScene->CreateObject("Square");         // 创建正方形物体
        m_SquareObject1.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));    // 添加 SpriteRenderer 组件

        m_SquareObject2 = m_ActiveScene->CreateObject("Square");         // 创建正方形物体
        m_SquareObject2.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));    // 添加 SpriteRenderer 组件

        m_CameraObject = m_ActiveScene->CreateObject("Main Camera");    // 创建相机对象
        m_CameraObject.AddComponent<CameraComponent>();                 // 添加 Camera 组件
        m_CameraObject.GetComponent<TransformComponent>().Transform.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));

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

        m_ActiveScene->OnUpdate(dt);// 更新场景

        m_Framebuffer->Unbind();    // 解除绑定帧缓冲区
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

        // 停靠空间 TODO ImGuiLayer
        ImGui::Begin("DockSpace", &dockSpaceOpen, windowFlags);
        {
            ImGui::PopStyleVar();

            if (optFullscreen)
            {
                ImGui::PopStyleVar(2);
            }

            ImGuiIO& io = ImGui::GetIO();

            ImGuiStyle& style = ImGui::GetStyle();      // 样式
            float minWinSizeX = style.WindowMinSize.x;  // 最小窗口大小
            style.WindowMinSize.x = 370.0f;

            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspaceID = ImGui::GetID("Editor Dockspace");
                ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockspaceFlags);
            }

            // 菜单条 TODO MenuBarPanel
            if (ImGui::BeginMenuBar())
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

                    //// 菜单项：加载场景
                    //if (ImGui::MenuItem("Open"))
                    //{
                    //    SceneSerializer serializer(m_ActiveScene);                  // m_ActiveScene 场景序列化器

                    //    serializer.Deserialize("Assets/Scenes/SampleScene.lucky");  // 反序列化
                    //}

                    //// 菜单项：保存场景
                    //if (ImGui::MenuItem("Save"))
                    //{
                    //    SceneSerializer serializer(m_ActiveScene);                  // m_ActiveScene 场景序列化器

                    //    serializer.Serialize("Assets/Scenes/SampleScene.lucky");    // 序列化
                    //}

                    // 菜单项：退出
                    if (ImGui::MenuItem("Quit"))
                    {
                        Application::GetInstance().Close();    // 退出程序
                    }

                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            m_HierarchyPanel.OnImGuiRender();   // 渲染 Hierarchy 面板
            m_PropertiesPanel.OnImGuiRender();  // 渲染 Properties 面板

            // 批渲染数据统计 TODO RendererStatsPanel
            ImGui::Begin("Renderer2D Stats");
            {
                auto stats = Renderer2D::GetStats();

                ImGui::Text("FPS: %.3f", fps);  // 帧率

                ImGui::Text("Draw Calls: %d", stats.DrawCalls);
                ImGui::Text("Quad: %d", stats.QuadCount);
                ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
                ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
            }
            ImGui::End();

            // 场景视口 TODO SceneViewportPanel
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0)); // 设置 Gui 窗口样式：边界 = 0
            ImGui::Begin("Scene");
            {
                m_ViewportFocused = ImGui::IsWindowFocused();   // 当前窗口被聚焦
                m_ViewportHovered = ImGui::IsWindowHovered();   // 鼠标悬停在当前窗口

                Application::GetInstance().GetImGuiLayer()->BlockEvents(/*!m_ViewportFocused ||*/ !m_ViewportHovered); // 阻止ImGui事件

                ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();      // 当前面板大小

                m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };  // 视口大小

                uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID(); // 颜色缓冲区 ID

                ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));   // 场景视口图像
            }
            ImGui::End();
            ImGui::PopStyleVar();
        }
        ImGui::End();
    }

    void EditorLayer::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<KeyPressedEvent>(LC_BIND_EVENT_FUNC(EditorLayer::OnKeyPressed));    // 调用按键按下事件
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
    }

    void EditorLayer::NewScene()
    {
        m_ActiveScene = CreateRef<Scene>();         // 创建新场景
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);    // 重置视口大小
        m_HierarchyPanel.SetScene(m_ActiveScene);   // 设置 Hierarchy 的场景
    }

    void EditorLayer::OpenScene()
    {
        std::string filepath = FileDialogs::OpenFile("Lucky Scene(*.lucky)\0*.lucky\0");    // 打开文件对话框（文件类型名\0 文件类型.lucky）
        
        // 路径不为空
        if (!filepath.empty())
        {
            m_ActiveScene = CreateRef<Scene>();         // 创建新场景
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);    // 重置视口大小
            m_HierarchyPanel.SetScene(m_ActiveScene);   // 设置 Hierarchy 的场景

            SceneSerializer serializer(m_ActiveScene);  // 场景序列化器
            serializer.Deserialize(filepath);           // 反序列化：加载文件场景到新场景
        }
    }

    void EditorLayer::SaveSceneAs()
    {
        std::string filepath = FileDialogs::SaveFile("Lucky Scene(*.lucky)\0*.lucky\0");    // 保存文件对话框（文件类型名\0 文件类型.lucky）
        // TODO 判断是否有.lucky后缀，没有则加上
        // 路径不为空
        if (!filepath.empty())
        {
            SceneSerializer serializer(m_ActiveScene);  // 场景序列化器
            serializer.Serialize(filepath);             // 序列化：保存场景
        }
    }
}