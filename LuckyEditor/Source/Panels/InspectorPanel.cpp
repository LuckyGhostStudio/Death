#include "InspectorPanel.h"

#include "Lucky/Scene/Components/SelfComponent.h"
#include "Lucky/Scene/Components/TransformComponent.h"
#include "Lucky/Scene/Components/CameraComponent.h"
#include "Lucky/Scene/Components/SpriteRendererComponent.h"

#include "Lucky/ImGui/GUI.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

namespace Lucky
{
    InspectorPanel::InspectorPanel()
        : EditorWindow("Inspector")
    {
    
    }

    InspectorPanel::~InspectorPanel()
    {

    }

    void InspectorPanel::OnUpdate(DeltaTime dt)
    {

    }

    void InspectorPanel::OnImGuiRender()
    {
        ImGui::Begin(m_Name.c_str());
        {
            // ��ѡ�е�������� TODO ����ʲ���Ϣ����
            if (m_SelectionObject)
            {
                DrawComponents(m_SelectionObject);  // ������� UI
            }
        }
        ImGui::End();
    }

    void InspectorPanel::OnEvent(Event& e)
    {

    }

    void InspectorPanel::AddComponents(Object object)
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

    void InspectorPanel::DrawComponents(Object object)
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

            GUI::DragVector3("Position", transform.GetPosition());  // λ�ã�Ĭ��ֵ 1.0f
            GUI::DragVector3("Rotation", transform.GetRotation());  // ��ת��Ĭ��ֵ 1.0f
            GUI::DragVector3("Scale", transform.GetScale(), 1.0f);  // ���ţ�Ĭ��ֵ 1.0f
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
                GUI::DragSlider("Vertical FOV", &camera.GetFOV_Ref(), 1.0f, 179.0f, GUI::ValueType::Angle); // FOV ��ֱ�Ž� ������
            }

            // ����ͶӰ
            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
            {
                GUI::DragFloat("Size", &camera.GetSize_Ref());  // �ߴ� �϶���
            }

            GUI::DragFloat("Near", &camera.GetNearClip_Ref(), 0.01f, GUI::ValueType::Float, 0.01f, camera.GetFarClip() - 0.01f);    // ���ü�ƽ��
            GUI::DragFloat("Far", &camera.GetFarClip_Ref(), 0.01f, GUI::ValueType::Float, camera.GetNearClip() + 0.01f, 1000.0f);   // Զ�ü�ƽ��
        });

        // SpriteRenderer ���
        DrawComponent<SpriteRendererComponent>("Sprite Renderer", object, [](SpriteRendererComponent& spriteRendererComponent)
        {
            ImGui::ColorEdit4("Color", glm::value_ptr(spriteRendererComponent.Color));   // ��ɫ�༭��
        });
    }
}