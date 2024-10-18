#include "EditorLayer.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>

#include "Lucky/Scene/SceneSerializer.h"
#include "Lucky/Utils/PlatformUtils.h"

#include "ImGuizmo.h"
#include "Lucky/Math/Math.h"

namespace Lucky
{
    EditorLayer::EditorLayer()
        : Layer("Editor")
    {

    }

    void EditorLayer::OnAttach()
    {
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
            
                // Gizmo
                Object selectedObject = Selection::Object;  // 被选中物体
                // 选中物体存在 && Gizmo 类型存在
                if (selectedObject && m_GizmoType != -1)
                {
                    ImGuizmo::SetOrthographic(false);   // 透视投影
                    ImGuizmo::SetDrawlist();            // 设置绘制列表

                    float windowWidth = (float)ImGui::GetWindowWidth();     // 视口宽
                    float windowHeight = (float)ImGui::GetWindowHeight();   // 视口高

                    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight); // 设置绘制区域

                    // Camera
                    Object cameraObject = m_ActiveScene->GetPrimaryCameraObject();              // 主相机对象
                    const auto& camera = cameraObject.GetComponent<CameraComponent>().Camera;   // 相机
                    const glm::mat4& cameraProjection = camera.GetProjectionMatrix();           // 投影矩阵
                    glm::mat4 cameraView = glm::inverse(cameraObject.GetComponent<TransformComponent>().Transform.GetTransform());  // 视图矩阵

                    // 被选中物体 transform
                    Transform& transformComponent = selectedObject.GetComponent<TransformComponent>().Transform;    // Transform
                    glm::mat4 transform = transformComponent.GetTransform();

                    bool span = Input::IsKeyPressed(Key::LeftControl);  // Ctrl 刻度捕捉：操作时固定 delta 刻度
                    float spanValue = 0.5f;     // 平移缩放间隔：0.5m

                    // 旋转间隔值：5 度
                    if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
                    {
                        spanValue = 5.0f;
                    }
                    
                    float spanValues[3] = { spanValue, spanValue, spanValue };  // xyz 轴刻度捕捉值

                    // 绘制操作 Gizmo：相机视图矩阵 相机投影矩阵 Gizmo 类型 本地 选中物体 transform 增量矩阵 刻度捕捉值
                    ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                        (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
                        nullptr, span ? spanValues : nullptr);

                    // Gizmo 被使用
                    if (ImGuizmo::IsUsing())
                    {
                        glm::vec3 position, rotation, scale;
                        Math::DecomposeTransform(transform, position, rotation, scale);         // 分解 transform 矩阵

                        glm::vec3 deltaRotation = rotation - transformComponent.GetRotation();  // 旋转增量

                        transformComponent.GetPosition() = position;        // 更新位置
                        transformComponent.GetRotation() += deltaRotation;  // 更新旋转：累加增量，避免万向节锁
                        transformComponent.GetScale() = scale;              // 更新缩放
                    }
                }
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

        // Gizmo 快捷键
        switch (e.GetKeyCode())
        {
            case Key::G:
                m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;   // 平移
                break;
            case Key::R:
                m_GizmoType = ImGuizmo::OPERATION::ROTATE;      // 旋转
                break;
            case Key::S:
                m_GizmoType = ImGuizmo::OPERATION::SCALE;       // 缩放
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