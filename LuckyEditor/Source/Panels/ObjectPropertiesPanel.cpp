#include "ObjectPropertiesPanel.h"

#include "Lucky/Scene/Components/SelfComponent.h"
#include "Lucky/Scene/Components/TransformComponent.h"
#include "Lucky/Scene/Components/CameraComponent.h"
#include "Lucky/Scene/Components/SpriteRendererComponent.h"

#include "Lucky/Scene/Selection.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Lucky
{
    void ObjectPropertiesPanel::OnImGuiRender()
    {
        // �������
        ImGui::Begin("Properties");
        {
            // ��ѡ�е�������� TODO ����ʲ���Ϣ����
            if (Selection::Object)
            {
                DrawComponents(Selection::Object);  // ������� UI
            }
        }
        ImGui::End();
    }

    void ObjectPropertiesPanel::AddComponents(Object object)
    {
        // ������
        if (ImGui::Button("Add Component"))
        {
            ImGui::OpenPopup("AddComponent");   // �򿪵�����
        }

        // ��Ⱦ������
        if (ImGui::BeginPopup("AddComponent"))
        {
            // ��� Camera ���
            if (ImGui::MenuItem("Camera"))
            {
                object.AddComponent<CameraComponent>();

                ImGui::CloseCurrentPopup();
            }

            // ��� SpriteRenderer ���
            if (ImGui::MenuItem("Sprite Renderer"))
            {
                object.AddComponent<SpriteRendererComponent>();

                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void ObjectPropertiesPanel::DrawComponents(Object object)
    {
        AddComponents(object);  // ������ UI

        // Self ���
        if (object.HasComponent<SelfComponent>())
        {
            auto& name = object.GetComponent<SelfComponent>().ObjectName;   // ������

            char buffer[256];                               // ��������� buffer
            memset(buffer, 0, sizeof(buffer));              // �� buffer ����
            strcpy_s(buffer, sizeof(buffer), name.c_str()); // buffer = name

            // �����������������ݸı�ʱ
            if (ImGui::InputText("Name", buffer, sizeof(buffer)))
            {
                name = std::string(buffer); // ���� name ��Ϊ buffer
            }
        }

        // ���ڵ��־����|�����ص�
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

        // Transform ���
        if (object.HasComponent<TransformComponent>())
        {
            // Transform �����㣺Transform �����Ĺ�ϣֵ��Ϊ��� id
            if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treeNodeFlags, "Transform"))
            {
                auto& transform = object.GetComponent<TransformComponent>().Transform;

                ImGui::DragFloat3("Position", glm::value_ptr(transform.GetPosition()), 0.1f);    //λ�ã��϶��ٶ�0.1
                ImGui::DragFloat3("Rotation", glm::value_ptr(transform.GetRotation()), 0.1f);    //��ת���϶��ٶ�0.1
                ImGui::DragFloat3("Scale", glm::value_ptr(transform.GetScale()), 0.1f);          //���ţ��϶��ٶ�0.1

                ImGui::TreePop();
            }
        }

        // Camera ���
        if (object.HasComponent<CameraComponent>())
        {
            // Camera ������
            if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), treeNodeFlags, "Camera"))
            {
                auto& camera= object.GetComponent<CameraComponent>().Camera;

                ImGui::Checkbox("Main Camera", &camera.IsPrimary_Ref());    // ��������ÿ�

                const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };                            // ͶӰ���ͣ�͸�� ���� 
                const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];   // ��ǰͶӰ����

                // ������ ѡ��ͶӰ����
                if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
                {
                    for (int i = 0; i < 2; i++)
                    {
                        bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];  // ��ѡ�У���ǰͶӰ����==��i��ͶӰ����
                        
                        // ��ѡ�������ı�ʱ��ͶӰ���� ��ѡ��
                        if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
                        {
                            currentProjectionTypeString = projectionTypeStrings[i];     // ���õ�ǰͶӰ����
                            camera.SetProjectionType((SceneCamera::ProjectionType)i);   // �������ͶӰ����
                        }
                        if (isSelected)
                        {
                            ImGui::SetItemDefaultFocus();   // ����Ĭ��ѡ����
                        }
                    }
                    ImGui::EndCombo();
                }

                // ͸��ͶӰ
                if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
                {
                    float verticalFov = camera.GetFOV();    // ��ֱ�Ž�

                    if (ImGui::SliderAngle("Vertical Fov", &verticalFov))
                    {
                        camera.SetFOV(verticalFov);
                    }
                }

                // ����ͶӰ
                if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
                {
                    float size = camera.GetSize();          // �ߴ�

                    if (ImGui::DragFloat("Size", &size))
                    {
                        camera.SetSize(size);
                    }
                }

                float nearClip = camera.GetNearClip();  // ���ü�ƽ��

                if (ImGui::DragFloat("Near", &nearClip))
                {
                    camera.SetNearClip(nearClip);
                }

                float farClip = camera.GetFarClip();    // Զ�ü�ƽ��

                if (ImGui::DragFloat("Far", &farClip))
                {
                    camera.SetFarClip(farClip);
                }

                ImGui::TreePop();
            }
        }

        // SpriteRenderer���
        if (object.HasComponent<SpriteRendererComponent>())
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));  // ���ñ߿���ʽ

            // SpriteRenderer ������
            bool opened = ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), treeNodeFlags, "Sprite Renderer");

            ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);   // ͬһ�У���ȡ���ڿ��
                
            // ������ð�ť
            if (ImGui::Button("+", ImVec2(20, 20)))
            {
                ImGui::OpenPopup("ComponentSettings");  // �򿪵�����
            }

            ImGui::PopStyleVar();

            // �Ƴ����
            bool componentRemoved = false;
            // ��Ⱦ������
            if (ImGui::BeginPopup("ComponentSettings"))
            {
                // �Ƴ�����˵���
                if (ImGui::MenuItem("Remove Component"))
                {
                    componentRemoved = true;    // ������Ϊ�Ƴ�
                }

                ImGui::EndPopup();
            }

            if (opened)
            {
                auto& spriteRenderer = object.GetComponent<SpriteRendererComponent>();

                ImGui::ColorEdit4("Color", glm::value_ptr(spriteRenderer.Color));   // ��ɫ�༭��

                ImGui::TreePop();
            }

            if (componentRemoved)
            {
                object.RemoveComponent<SpriteRendererComponent>();  // �Ƴ� SpriteRenderer ���
            }
        }
    }
}
