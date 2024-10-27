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

        m_EditorCamera = EditorCamera(30.0f, 1280.0f / 720.0f, 0.01f, 1000.0f); // 创建编辑器相机

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
        if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);  // 重置帧缓冲区大小

            m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);                         // 设置视口大小
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);    // 重置视口大小
        }

        // 视口被聚焦
        if (m_ViewportFocused)
        {

        }

        m_EditorCamera.OnUpdate(dt);    // 更新编辑器相机

        Renderer2D::ResetStats();   // 重置统计数据

        m_Framebuffer->Bind();      // 绑定帧缓冲区

        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        RenderCommand::Clear();

        m_Framebuffer->ClearAttachment(1, -1);  // 清除颜色缓冲区 1（物体 id 缓冲区）为 -1

        m_ActiveScene->OnUpdateEditor(dt, m_EditorCamera);  // 更新编辑器场景

        auto [mx, my] = ImGui::GetMousePos();   // 鼠标位置

        // 计算鼠标相对于视口左上角的位置
        mx -= m_ViewportBounds[0].x;
        my -= m_ViewportBounds[0].y;

        glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0]; // 视口大小：包括 tab bar

        my = viewportSize.y - my;   // 翻转 y 坐标 左下角0,0（纹理坐标左下角为0,0）

        // 鼠标相对于视口左上角的位置
        int mouseX = (int)mx;
        int mouseY = (int)my;
        
        // 鼠标在视口内
        if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
        {
            int pixelData = m_Framebuffer->GetPixel(1, mouseX, mouseY); // 读取 1 号颜色缓冲区像素
            // 被鼠拾取的物体（不是 -1）
            m_PickedObject = pixelData == -1 ? Object() : Object((entt::entity)pixelData, m_ActiveScene.get());

            LC_CORE_WARN("pixelData:{0}", pixelData);
        }

        m_Framebuffer->Unbind();    // 解除绑定帧缓冲区
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

        m_HierarchyPanel.OnImGuiRender();   // 渲染 Hierarchy 面板
        m_PropertiesPanel.OnImGuiRender();  // 渲染 Properties 面板

        // 批渲染数据统计 TODO RendererStatsPanel
        ImGui::Begin("Scece Renderer Stats");
        {
            auto stats = Renderer2D::GetStats();

            ImGui::Text("FPS: %.3f", Application::GetInstance().GetFPS());  // 帧率

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
            auto viewportMinRegion = ImGui::GetWindowContentRegionMin();    // 视口可用区域最小值（视口左上角相对于视口左上角位置）
            auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();    // 视口可用区域最大值（视口右下角相对于视口左上角位置）
            auto viewportOffset = ImGui::GetWindowPos();                    // 视口偏移量：视口面板左上角位置（相对于屏幕左上角）

            m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
            m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

            m_ViewportFocused = ImGui::IsWindowFocused();   // 当前窗口被聚焦
            m_ViewportHovered = ImGui::IsWindowHovered();   // 鼠标悬停在当前窗口

            Application::GetInstance().GetImGuiLayer()->BlockEvents(/*!m_ViewportFocused ||*/ !m_ViewportHovered); // 阻止ImGui事件

            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();      // 当前面板大小

            m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };  // 视口大小

            uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID(); // 颜色缓冲区 0 ID

            ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));   // 场景视口图像

            // Gizmo
            Object selectedObject = Selection::Object;  // 被选中物体
            // 选中物体存在 && Gizmo 类型存在
            if (selectedObject && m_GizmoType != -1)
            {
                ImGuizmo::SetOrthographic(false);   // 透视投影
                ImGuizmo::SetDrawlist();            // 设置绘制列表

                // 设置绘制区域
                ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

                // 编辑器相机
                const glm::mat4& cameraProjection = m_EditorCamera.GetProjectionMatrix();   // 投影矩阵
                glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();                      // 视图矩阵

                // 绘制坐标系
                //ImGuizmo::DrawGrid(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), glm::value_ptr(glm::mat4(1.0f)), 10);

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

                // 绘制操作 Gizmo：相机视图矩阵 相机投影矩阵 Gizmo 类型 本地坐标系 选中物体 transform 增量矩阵 刻度捕捉值
                ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                    (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
                    nullptr, span ? spanValues : nullptr);

                // Gizmo 正在使用
                if (ImGuizmo::IsUsing())
                {
                    glm::vec3 position, rotation, scale;
                    Math::DecomposeTransform(transform, position, rotation, scale); // 分解 transform 矩阵
                        
                    switch (m_GizmoType)
                    {
                        case ImGuizmo::OPERATION::TRANSLATE:
                            transformComponent.GetPosition() = position;        // 更新位置
                            break;
                        case ImGuizmo::OPERATION::ROTATE:
                            glm::vec3 deltaRotation = rotation - transformComponent.GetRotation();  // 旋转增量
                            transformComponent.GetRotation() += deltaRotation;  // 更新旋转：累加增量，避免万向节锁
                            break;
                        case ImGuizmo::OPERATION::SCALE:
                            transformComponent.GetScale() = scale;              // 更新缩放
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
        m_EditorCamera.OnEvent(event);  // 编辑器相机事件

        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<KeyPressedEvent>(LC_BIND_EVENT_FUNC(EditorLayer::OnKeyPressed));                    // 调用按键按下事件
        dispatcher.Dispatch<MouseButtonPressedEvent>(LC_BIND_EVENT_FUNC(EditorLayer::OnMouseButtonPressed));    // 调用鼠标按钮按下事件
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

        // Gizmo 不是正在使用
        if (!ImGuizmo::IsUsing())
        {
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

        return false;
    }

    bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        // 鼠标左键按下
        if (e.GetMouseButton() == Mouse::ButtonLeft)
        {
            // 鼠标在视口内 Gizmo 控制未结束
            if (m_ViewportHovered && !ImGuizmo::IsOver())
            {
                Selection::Object = m_PickedObject; // 设置被选中物体
            }
        }

        return false;
    }

    void EditorLayer::NewScene()
    {
        m_ActiveScene = CreateRef<Scene>();         // 创建新场景
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);    // 重置视口大小
        m_HierarchyPanel.SetScene(m_ActiveScene);   // 设置 Hierarchy 的场景
    }

    void EditorLayer::OpenScene()
    {
        std::optional<std::string> filepath = FileDialogs::OpenFile("Lucky Scene(*.lucky)\0*.lucky\0");    // 打开文件对话框（文件类型名\0 文件类型.lucky）
        
        // 路径不为空
        if (filepath.has_value())
        {
            m_ActiveScene = CreateRef<Scene>();         // 创建新场景
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);    // 重置视口大小
            m_HierarchyPanel.SetScene(m_ActiveScene);   // 设置 Hierarchy 的场景

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