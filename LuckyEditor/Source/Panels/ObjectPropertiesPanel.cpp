#include "ObjectPropertiesPanel.h"

#include "Lucky/Scene/Components/SelfComponent.h"
#include "Lucky/Scene/Components/TransformComponent.h"
#include "Lucky/Scene/Components/CameraComponent.h"
#include "Lucky/Scene/Components/SpriteRendererComponent.h"

#include "Lucky/Scene/Selection.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
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

    /// <summary>
    /// ����Vector3�ؼ�
    /// </summary>
    /// <param name="label">��ǩ</param>
    /// <param name="values">ֵ</param>
    /// <param name="resetValue">����ֵ</param>
    /// <param name="columnWidth">ÿ�п��</param>
    static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
    {
        ImGuiIO& io = ImGui::GetIO();
        auto boldFont = io.Fonts->Fonts[0]; // ���壺0������

        ImGui::PushID(label.c_str());   // ���ÿؼ���ǩ

        // ��ǩ��
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);  // ���� 0 ���п�
        ImGui::Text(label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());         // ���� 3 ���п�
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 }); // Var ��ʽ

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;    // �и� = �����С + �߿�.y * 2
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };  // ����ֵ��ť��С

        // X ���� UI
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));        // ��ť��ɫ
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.3f, 0.35f, 1.0f)); // �����ͣ�ڰ�ťʱ����ɫ
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.1f, 0.15f, 1.0f));  // ��ť������ɫ

        ImGui::PushFont(boldFont);  // ���ô���

        // X ��ť����
        if (ImGui::Button("X", buttonSize))
        {
            values.x = resetValue;  // ���� x ����
        }

        ImGui::PopFont();

        ImGui::PopStyleColor(3);

        ImGui::SameLine();                          // ��ͬһ��
        ImGui::DragFloat("##X", &values.x, 0.1f);   // X ������ ##����ʾ��ǩ �϶����� 0.1
        ImGui::PopItemWidth();                      // �Ƴ���һ���п�
        ImGui::SameLine();

        // Y ���� UI
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));         // ��ť��ɫ
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.9f, 0.4f, 1.0f));  // �����ͣ�ڰ�ťʱ����ɫ
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));   // ��ť������ɫ

        ImGui::PushFont(boldFont);  // ���ô���

        // Y ��ť����
        if (ImGui::Button("Y", buttonSize))
        {
            values.y = resetValue;  // ���� y ����
        }

        ImGui::PopFont();

        ImGui::PopStyleColor(3);

        ImGui::SameLine();                          // ��ͬһ��
        ImGui::DragFloat("##Y", &values.y, 0.1f);   // Y ������ ##����ʾ��ǩ �϶����� 0.1
        ImGui::PopItemWidth();                      // �Ƴ���һ���п�
        ImGui::SameLine();

        // Z ���� UI
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.25f, 0.8f, 1.0f));        // ��ť��ɫ
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.4f, 0.8f, 1.0f));  // �����ͣ�ڰ�ťʱ����ɫ
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35f, 0.25f, 0.9f, 1.0f)); // ��ť������ɫ

        ImGui::PushFont(boldFont);  // ���ô���

        // Z ��ť����
        if (ImGui::Button("Z", buttonSize))
        {
            values.z = resetValue;  // ���� z ����
        }

        ImGui::PopFont();

        ImGui::PopStyleColor(3);

        ImGui::SameLine();                          // ��ͬһ��
        ImGui::DragFloat("##Z", &values.z, 0.1f);   // Z ������ ##����ʾ��ǩ �϶����� 0.1
        ImGui::PopItemWidth();                      // �Ƴ���һ���п�

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();     // �����ؼ���ǩ
    }

    /// <summary>
    /// �������
    /// </summary>
    /// <typeparam name="T">�������</typeparam>
    /// <typeparam name="UIFunction">������ܺ�������</typeparam>
    /// <param name="name">�����</param>
    /// <param name="object">ʵ��</param>
    /// <param name="uiFunction">������ܺ���</param>
    template<typename T, typename UIFunction>
    static void DrawComponent(const std::string& name, Object object, UIFunction uiFunction)
    {
        // ���ڵ��־����|���|���쵽�ұ�|�����ص�|��ܱ߿�
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

        // T �������
        if (object.HasComponent<T>())
        {
            auto& component = object.GetComponent<T>();    // ������

            ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();     // ���������С

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));  // ���ñ߿���ʽ
            float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;    //�и� = �����С + �߿� y * 2
            ImGui::Separator();     // �ָ���

            // �����㣺�����Ĺ�ϣֵ��Ϊ��� id
            bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
            ImGui::PopStyleVar();

            ImGui::SameLine(contentRegionAvail.x - lineHeight * 0.5f);      // ͬһ�У�������������

            // ������ð�ť
            if (ImGui::Button("+", ImVec2(lineHeight, lineHeight)))
            {
                ImGui::OpenPopup("ComponentSettings");  // �򿪵�����
            }

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
                uiFunction(component);  // ����������ܺ��������Ƹ������ͬ�Ĳ���

                ImGui::TreePop();       // չ�����
            }

            if (componentRemoved)
            {
                object.RemoveComponent<T>();    // �Ƴ� T ���
            }
        }
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
            if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
            {
                name = std::string(buffer); // ���� name ��Ϊ buffer
            }
        }

        ImGui::SameLine();
        ImGui::PushItemWidth(-1);   // �����ұ߿��-1

        AddComponents(object);      // ������ UI

        // ���ڵ��־����|�����ص�
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

        // Transform ���
        DrawComponent<TransformComponent>("Transform", object, [](TransformComponent& transformComponent)
        {
            auto& transform = transformComponent.Transform;

            DrawVec3Control("Position", transform.GetPosition());   // λ�ã�Ĭ��ֵ 1.0f
            DrawVec3Control("Rotation", transform.GetRotation());   // ��ת��Ĭ��ֵ 1.0f
            DrawVec3Control("Scale", transform.GetScale(), 1.0f);   // ���ţ�Ĭ��ֵ 1.0f
        });

        // Camera ���
        DrawComponent<CameraComponent>("Camera", object, [](CameraComponent& cameraComponent)
        {
            auto& camera = cameraComponent.Camera;

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
                float size = camera.GetSize();      // �ߴ�

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
        });

        // SpriteRenderer ���
        DrawComponent<SpriteRendererComponent>("SpriteRenderer", object, [](SpriteRendererComponent& spriteRendererComponent)
        {
            ImGui::ColorEdit4("Color", glm::value_ptr(spriteRendererComponent.Color));   // ��ɫ�༭��
        });
    }
}