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
            m_Framebuffer->Resize((uint32_t)m_Size.x, (uint32_t)m_Size.y);      // ����֡��������С
            m_EditorCamera.SetViewportSize(m_Size.x, m_Size.y);                 // ���ñ༭������ӿڴ�С
        }

        // �ӿڱ��۽�
        if (m_IsFocused)
        {

        }

        m_EditorCamera.OnUpdate(dt);    // ���±༭�����

        m_Framebuffer->Bind();      // ��֡������

        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        RenderCommand::Clear();

        m_Framebuffer->ClearAttachment(1, -1);  // �����ɫ������ 1������ id ��������Ϊ -1

        m_Scene->OnUpdateEditor(dt, m_EditorCamera);  // ���±༭������

        //m_Size = m_Bounds[1] - m_Bounds[0];     // �ӿڴ�С������ tab bar
        
        auto [mx, my] = ImGui::GetMousePos();   // ���λ��

        // �������������ӿ����Ͻǵ�λ��
        mx -= m_Bounds[0].x;
        my -= m_Bounds[0].y;

        my = m_Size.y - my;   // ��ת y ���� ���½�0,0�������������½�Ϊ0,0��

        // ���������ӿ����Ͻǵ�λ��
        int mouseX = (int)mx;
        int mouseY = (int)my;

        // ������ӿ���
        if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)m_Size.x && mouseY < (int)m_Size.y)
        {
            int pixelData = m_Framebuffer->GetPixel(1, mouseX, mouseY); // ��ȡ 1 ����ɫ����������
            // ����ʰȡ�����壨���� -1��
            m_PickedObject = pixelData == -1 ? Object() : Object((entt::entity)pixelData, m_Scene.get());

            // LC_CORE_WARN("pixelData:{0}", pixelData);
        }

        OnGizmosRender();    // ���� Gizmos

        m_Framebuffer->Unbind();    // �����֡������
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
                // ��ǰѡ����
                if (Selection::Object == obj)
                {
                    auto [transformComponent, boxCollider2DComponent] = view.get<TransformComponent, BoxCollider2DComponent>(entity);

                    Transform& transform = transformComponent.Transform;
                    BoxCollider2D& boxCollider2D = boxCollider2DComponent.BoxCollider2d;

                    // BoxCollider2D ����� obj �� Transform TODO bug: Offset �� Scale ͬʱ�ı䣬����λ�û᲻��ȷ
                    glm::mat4 boxLocalTransform = glm::translate(glm::mat4(1.0f), glm::vec3(boxCollider2D.Offset, 0.001f))
                        * glm::scale(glm::mat4(1.0f), glm::vec3(boxCollider2D.Size, 1.0f));

                    glm::mat4 trans = transform.GetTransform() * boxLocalTransform; // ���� Transform

                    Renderer2D::DrawRect(trans, glm::vec4(0.5686f, 0.9569f, 0.5451f, 1));   // ���� BoxCollider2D
                }
            }
        }

        // Circle Collider 2D
        {
            auto view = m_Scene->GetAllEntitiesWith<TransformComponent, CircleCollider2DComponent>();
            for (auto entity : view)
            {
                Object obj = { entity, m_Scene.get() };
                // ��ǰѡ����
                if (Selection::Object == obj)
                {
                    auto [transformComponent, circleCollider2DComponent] = view.get<TransformComponent, CircleCollider2DComponent>(entity);

                    Transform& transform = transformComponent.Transform;
                    CircleCollider2D& circleCollider2D = circleCollider2DComponent.CircleCollider2d;

                    // obj ���� Scale ��� Transform TODO bug: Offset �� Scale ͬʱ�ı䣬����λ�û᲻��ȷ
                    glm::mat4 parentTransform = glm::translate(glm::mat4(1.0f), transform.GetPosition())
                        * glm::toMat4(glm::quat(glm::radians(transform.GetRotation())))
                        * glm::scale(glm::mat4(1.0f), glm::vec3(glm::max(glm::abs(transform.GetScale().x), glm::abs(transform.GetScale().y))));
                    // CircleCollider2D ����� obj ��Transform
                    glm::mat4 circLocalTransform = glm::translate(glm::mat4(1.0f), glm::vec3(circleCollider2D.Offset, 0.001f))
                        * glm::scale(glm::mat4(1.0f), glm::vec3(circleCollider2D.Radius));

                    glm::mat4 trans = parentTransform * circLocalTransform; // ���� Transform

                    Renderer2D::DrawCircle(trans, glm::vec4(0.5686f, 0.9569f, 0.5451f, 1));   // ���� CircleCollider2D
                }
            }
        }

        Renderer2D::EndScene();
    }

    void SceneViewportPanel::OnImGuiRender(bool& isOpen)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1, 0)); // ���� Gui ������ʽ���߽� = 0
        ImGui::Begin("Scene");
        {
            auto viewportMinRegion = ImGui::GetWindowContentRegionMin();    // �ӿڿ���������Сֵ���ӿ����Ͻ�������ӿ����Ͻ�λ�ã�
            auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();    // �ӿڿ����������ֵ���ӿ����½�������ӿ����Ͻ�λ�ã�
            auto viewportOffset = ImGui::GetWindowPos();                    // �ӿ�ƫ�������ӿ�������Ͻ�λ�ã��������Ļ���Ͻǣ�

            m_Bounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
            m_Bounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

            m_IsFocused = ImGui::IsWindowFocused();   // ��ǰ���ڱ��۽�
            m_IsHovered = ImGui::IsWindowHovered();   // �����ͣ�ڵ�ǰ����

            //Application::GetInstance().GetImGuiLayer()->BlockEvents(/*!m_ViewportFocused ||*/ !m_IsHovered); // ��ֹImGui�¼�

            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();  // ��ǰ����С
            m_Size = { viewportPanelSize.x, viewportPanelSize.y };      // �ӿڴ�С

            uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID(); // ��ɫ������ 0 ID

            ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_Size.x, m_Size.y }, ImVec2(0, 1), ImVec2(1, 0));   // �����ӿ�ͼ��

            // Gizmo
            Object selectedObject = Selection::Object;  // ��ѡ������
            // ѡ��������� && Gizmo ���ʹ���
            if (selectedObject && m_GizmoType != -1)
            {
                ImGuizmo::SetOrthographic(false);   // ͸��ͶӰ
                ImGuizmo::SetDrawlist();            // ���û����б�

                // ���û�������
                ImGuizmo::SetRect(m_Bounds[0].x, m_Bounds[0].y, m_Bounds[1].x - m_Bounds[0].x, m_Bounds[1].y - m_Bounds[0].y);

                // �༭�����
                const glm::mat4& cameraProjection = m_EditorCamera.GetProjectionMatrix();   // ͶӰ����
                glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();                      // ��ͼ����

                // ��������ϵ
                //ImGuizmo::DrawGrid(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), glm::value_ptr(glm::mat4(1.0f)), 10);

                // ��ѡ������ transform
                Transform& transformComponent = selectedObject.GetComponent<TransformComponent>().Transform; // Transform
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

    void SceneViewportPanel::OnEvent(Event& e)
    {
        if (!m_IsHovered)
        {
            return;
        }

        m_EditorCamera.OnEvent(e);      // �༭������¼�

        EventDispatcher dispatcher(e);

        dispatcher.Dispatch<KeyPressedEvent>(LC_BIND_EVENT_FUNC(SceneViewportPanel::OnKeyPressed));                 // ���ð��������¼�
        dispatcher.Dispatch<MouseButtonPressedEvent>(LC_BIND_EVENT_FUNC(SceneViewportPanel::OnMouseButtonPressed)); // ������갴ť�����¼�
    }

    bool SceneViewportPanel::OnKeyPressed(KeyPressedEvent& e)
    {
        // �����ظ�
        if (e.IsRepeat())
        {
            return false;
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

    bool SceneViewportPanel::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        // ����������
        if (e.GetMouseButton() == Mouse::ButtonLeft)
        {
            // ������ӿ��� ���û��λ�� Gizmo ��
            if (m_IsHovered && !ImGuizmo::IsOver())
            {
                Selection::Object = m_PickedObject;  // ���� Selection
            }
        }

        return false;
    }
}