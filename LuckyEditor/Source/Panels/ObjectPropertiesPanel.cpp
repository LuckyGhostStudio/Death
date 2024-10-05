#include "ObjectPropertiesPanel.h"

#include "Lucky/Scene/Components/SelfComponent.h"
#include "Lucky/Scene/Components/TransformComponent.h"
#include "Lucky/Scene/Components/CameraComponent.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Lucky
{
    ObjectPropertiesPanel::ObjectPropertiesPanel(Selection selection)
    {
        SetSelection(selection);
    }

    void ObjectPropertiesPanel::SetSelection(Selection selection)
    {
        m_Selection = selection;
    }

    void ObjectPropertiesPanel::OnImGuiRender()
    {
        // �������
        ImGui::Begin("Properties");
        {
            // ��ѡ�е�������� TODO ����ʲ���Ϣ����
            if (m_Selection.GetSelection())
            {
                DrawComponents(m_Selection.GetSelection()); // �������
            }
        }
        ImGui::End();
    }

    void ObjectPropertiesPanel::DrawComponents(Object object)
    {
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

        // Transform ���
        if (object.HasComponent<TransformComponent>())
        {
            // Transform �����㣺Transform �����Ĺ�ϣֵ��Ϊ��� id Ĭ�ϴ�
            if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
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
            if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera")) 
            {
                auto& cameraComponent = object.GetComponent<CameraComponent>();
                SceneCamera& camera = cameraComponent.Camera;

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
    }
}
