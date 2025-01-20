#include "SceneViewportPanel.h"

#include "Lucky/Core/Core.h"
#include "Lucky/Core/Application.h"
#include "Lucky/Core/Input/Input.h"
#include "Lucky/Renderer/RenderCommand.h"
#include "Lucky/Renderer/Renderer2D.h"

#include "Lucky/Scene/Components/TransformComponent.h"
#include "Lucky/Scene/Components/BoxCollider2DComponent.h"
#include "Lucky/Scene/Components/CircleCollider2DComponent.h"

#include <imgui/imgui.h>
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>

#include "Lucky/Math/Math.h"

namespace Lucky
{
    SceneViewportPanel::SceneViewportPanel(const Ref<Framebuffer>& framebuffer, const Ref<Scene>& scene)
        : m_Framebuffer(framebuffer),
        m_Scene(scene),
        m_EditorCamera(30.0f, 1280.0f / 720.0f, 0.01f, 1000.0f),
        m_PickedObject({})
    {

    }

    void SceneViewportPanel::SetSceneContext(const Ref<Scene>& scene)
    {
        m_Scene = scene;
        m_PickedObject = {};
    }

    void SceneViewportPanel::OnUpdate(DeltaTime dt)
    {
        if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
            m_Size.x > 0.0f && m_Size.y > 0.0f &&
            (spec.Width != m_Size.x || spec.Height != m_Size.y))
        {
            m_Framebuffer->Resize((uint32_t)m_Size.x, (uint32_t)m_Size.y);      // 重置帧缓冲区大小
            m_EditorCamera.SetViewportSize(m_Size.x, m_Size.y);                 // 重置编辑器相机视口大小
        }

        // 视口被聚焦
        if (m_IsFocused)
        {

        }

        m_EditorCamera.OnUpdate(dt);    // 更新编辑器相机

        m_Framebuffer->Bind();      // 绑定帧缓冲区

        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        RenderCommand::Clear();

        m_Framebuffer->ClearAttachment(1, -1);  // 清除颜色缓冲区 1（物体 id 缓冲区）为 -1

        m_Scene->OnUpdateEditor(dt, m_EditorCamera);  // 更新编辑器场景

        //m_Size = m_Bounds[1] - m_Bounds[0];     // 视口大小：包括 tab bar
        
        auto [mx, my] = ImGui::GetMousePos();   // 鼠标位置

        // 计算鼠标相对于视口左上角的位置
        mx -= m_Bounds[0].x;
        my -= m_Bounds[0].y;

        my = m_Size.y - my;   // 翻转 y 坐标 左下角0,0（纹理坐标左下角为0,0）

        // 鼠标相对于视口左上角的位置
        int mouseX = (int)mx;
        int mouseY = (int)my;

        // 鼠标在视口内
        if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)m_Size.x && mouseY < (int)m_Size.y)
        {
            int pixelData = m_Framebuffer->GetPixel(1, mouseX, mouseY); // 读取 1 号颜色缓冲区像素
            // 被鼠拾取的物体（不是 -1）
            m_PickedObject = pixelData == -1 ? Object() : Object((entt::entity)pixelData, m_Scene.get());

            // LC_CORE_WARN("pixelData:{0}", pixelData);
        }

        OnGizmosRender();    // 绘制 Gizmos

        m_Framebuffer->Unbind();    // 解除绑定帧缓冲区
    }

    void SceneViewportPanel::OnGizmosRender()
    {
        if (!m_ShowGizmos) return;

        Renderer2D::BeginScene(m_EditorCamera);

        // TODO Camera

        // Box Collider 2D
        {
            auto view = m_Scene->GetAllEntitiesWith<TransformComponent, BoxCollider2DComponent>();
            for (auto entity : view)
            {
                Object obj = { entity, m_Scene.get() };
                // 当前选中项
                if (Selection::Object == obj)
                {
                    auto [transformComponent, boxCollider2DComponent] = view.get<TransformComponent, BoxCollider2DComponent>(entity);

                    Transform& transform = transformComponent.Transform;
                    BoxCollider2D& boxCollider2D = boxCollider2DComponent.BoxCollider2d;

                    // BoxCollider2D 相对于 obj 的 Transform TODO bug: Offset 和 Scale 同时改变，刚体位置会不正确
                    glm::mat4 boxLocalTransform = glm::translate(glm::mat4(1.0f), glm::vec3(boxCollider2D.Offset, 0.001f))
                        * glm::scale(glm::mat4(1.0f), glm::vec3(boxCollider2D.Size, 1.0f));

                    glm::mat4 trans = transform.GetTransform() * boxLocalTransform; // 最终 Transform

                    Renderer2D::DrawRect(trans, glm::vec4(0.5686f, 0.9569f, 0.5451f, 1));   // 绘制 BoxCollider2D
                }
            }
        }

        // Circle Collider 2D
        {
            auto view = m_Scene->GetAllEntitiesWith<TransformComponent, CircleCollider2DComponent>();
            for (auto entity : view)
            {
                Object obj = { entity, m_Scene.get() };
                // 当前选中项
                if (Selection::Object == obj)
                {
                    auto [transformComponent, circleCollider2DComponent] = view.get<TransformComponent, CircleCollider2DComponent>(entity);

                    Transform& transform = transformComponent.Transform;
                    CircleCollider2D& circleCollider2D = circleCollider2DComponent.CircleCollider2d;

                    // obj 调整 Scale 后的 Transform TODO bug: Offset 和 Scale 同时改变，刚体位置会不正确
                    glm::mat4 parentTransform = glm::translate(glm::mat4(1.0f), transform.GetPosition())
                        * glm::toMat4(glm::quat(glm::radians(transform.GetRotation())))
                        * glm::scale(glm::mat4(1.0f), glm::vec3(glm::max(glm::abs(transform.GetScale().x), glm::abs(transform.GetScale().y))));
                    // CircleCollider2D 相对于 obj 的Transform
                    glm::mat4 circLocalTransform = glm::translate(glm::mat4(1.0f), glm::vec3(circleCollider2D.Offset, 0.001f))
                        * glm::scale(glm::mat4(1.0f), glm::vec3(circleCollider2D.Radius));

                    glm::mat4 trans = parentTransform * circLocalTransform; // 最终 Transform

                    Renderer2D::DrawCircle(trans, glm::vec4(0.5686f, 0.9569f, 0.5451f, 1));   // 绘制 CircleCollider2D
                }
            }
        }

        Renderer2D::EndScene();
    }

    void SceneViewportPanel::OnImGuiRender(bool& isOpen)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1, 0)); // 设置 Gui 窗口样式：边界 = 0
        ImGui::Begin("Scene");
        {
            auto viewportMinRegion = ImGui::GetWindowContentRegionMin();    // 视口可用区域最小值（视口左上角相对于视口左上角位置）
            auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();    // 视口可用区域最大值（视口右下角相对于视口左上角位置）
            auto viewportOffset = ImGui::GetWindowPos();                    // 视口偏移量：视口面板左上角位置（相对于屏幕左上角）

            m_Bounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
            m_Bounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

            m_IsFocused = ImGui::IsWindowFocused();   // 当前窗口被聚焦
            m_IsHovered = ImGui::IsWindowHovered();   // 鼠标悬停在当前窗口

            //Application::GetInstance().GetImGuiLayer()->BlockEvents(/*!m_ViewportFocused ||*/ !m_IsHovered); // 阻止ImGui事件

            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();  // 当前面板大小
            m_Size = { viewportPanelSize.x, viewportPanelSize.y };      // 视口大小

            uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID(); // 颜色缓冲区 0 ID

            ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_Size.x, m_Size.y }, ImVec2(0, 1), ImVec2(1, 0));   // 场景视口图像

            // Gizmo
            Object selectedObject = Selection::Object;  // 被选中物体
            // 选中物体存在 && Gizmo 类型存在
            if (selectedObject && m_GizmoType != -1)
            {
                ImGuizmo::SetOrthographic(false);   // 透视投影
                ImGuizmo::SetDrawlist();            // 设置绘制列表

                // 设置绘制区域
                ImGuizmo::SetRect(m_Bounds[0].x, m_Bounds[0].y, m_Bounds[1].x - m_Bounds[0].x, m_Bounds[1].y - m_Bounds[0].y);

                // 编辑器相机
                const glm::mat4& cameraProjection = m_EditorCamera.GetProjectionMatrix();   // 投影矩阵
                glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();                      // 视图矩阵

                // 绘制坐标系
                //ImGuizmo::DrawGrid(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), glm::value_ptr(glm::mat4(1.0f)), 10);

                // 被选中物体 transform
                Transform& transformComponent = selectedObject.GetComponent<TransformComponent>().Transform; // Transform
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

    void SceneViewportPanel::OnEvent(Event& e)
    {
        if (!m_IsHovered)
        {
            return;
        }

        m_EditorCamera.OnEvent(e);      // 编辑器相机事件

        EventDispatcher dispatcher(e);

        dispatcher.Dispatch<KeyPressedEvent>(LC_BIND_EVENT_FUNC(SceneViewportPanel::OnKeyPressed));                 // 调用按键按下事件
        dispatcher.Dispatch<MouseButtonPressedEvent>(LC_BIND_EVENT_FUNC(SceneViewportPanel::OnMouseButtonPressed)); // 调用鼠标按钮按下事件
    }

    bool SceneViewportPanel::OnKeyPressed(KeyPressedEvent& e)
    {
        // 按键重复
        if (e.IsRepeat())
        {
            return false;
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

    bool SceneViewportPanel::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        // 鼠标左键按下
        if (e.GetMouseButton() == Mouse::ButtonLeft)
        {
            // 鼠标在视口内 鼠标没有位于 Gizmo 上
            if (m_IsHovered && !ImGuizmo::IsOver())
            {
                Selection::Object = m_PickedObject;  // 更新 Selection
            }
        }

        return false;
    }
}