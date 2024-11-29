#include "InspectorPanel.h"

// #include <imgui/imgui.h>
// #include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

#include "Lucky/Scene/Components/SelfComponent.h"
#include "Lucky/Scene/Components/TransformComponent.h"
#include "Lucky/Scene/Components/CameraComponent.h"
#include "Lucky/Scene/Components/SpriteRendererComponent.h"
#include "Lucky/Scene/Components/Rigidbody2DComponent.h"
#include "Lucky/Scene/Components/BoxCollider2DComponent.h"
#include "Lucky/Scene/Components/CircleCollider2DComponent.h"

#include "Lucky/ImGui/GUI.h"

#include "Lucky/Utils/PlatformUtils.h"

namespace Lucky
{
    InspectorPanel::InspectorPanel()
    {
        m_SettingsButtonIcon = Texture2D::Create("Resources/Icons/SettingsButton_Icon.png");

        m_TransformIcon = Texture2D::Create("Resources/Icons/Components/Transform_Icon.png");
        m_CameraIcon = Texture2D::Create("Resources/Icons/Components/Camera_Icon.png");
        m_SpriteRendererIcon = Texture2D::Create("Resources/Icons/Components/SpriteRenderer_Icon.png");
        m_Rigidbody2DIcon = Texture2D::Create("Resources/Icons/Components/Rigidbody_Icon.png");
        m_BoxCollider2DIcon = Texture2D::Create("Resources/Icons/Components/BoxCollider2D_Icon.png");
    }

    void InspectorPanel::OnImGuiRender(bool& isOpen)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 10));    // ���� Padding���ؼ��߽絽���ڱ߽�ľ��룩
        ImGui::Begin("Inspector"/*, &isOpen*/);
        {
            ImGui::PopStyleVar();

            m_IsFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 8));   // ��ֱ���Ϊ 4

            // ��ѡ�е�������� TODO ����ʲ���Ϣ����
            if (m_SelectionObject)
            {
                DrawComponents(m_SelectionObject);  // ������� UI
            }

            ImGui::PopStyleVar();
        }
        ImGui::End();
    }

    void InspectorPanel::AddComponents(Object object)
    {
        float panelWidth = ImGui::GetWindowContentRegionWidth() + ImGui::GetStyle().WindowPadding.x * 2;    // �����

        // ������е�λ��
        float posX = (panelWidth - 300) * 0.5f;

        // ���ð�ť��λ��
        ImGui::SetCursorPosX(posX);

        // ������
        if (ImGui::Button("Add Component", ImVec2(300, 0)))
        {
            ImGui::OpenPopup("AddComponent");   // �򿪵�����
        }

        // ��Ⱦ������ TODO �������Ƿ������Ӷ��
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

            // ��� Rigidbody2D ���
            if (ImGui::MenuItem("Rigidbody 2D"))
            {
                object.AddComponent<Rigidbody2DComponent>();

                ImGui::CloseCurrentPopup();
            }

            // ��� BoxCollider2D ���
            if (ImGui::MenuItem("Box Collider 2D"))
            {
                object.AddComponent<BoxCollider2DComponent>();

                ImGui::CloseCurrentPopup();
            }

            // ��� CircleCollider2D ���
            if (ImGui::MenuItem("Circle Collider 2D"))
            {
                object.AddComponent<CircleCollider2DComponent>();

                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void InspectorPanel::DrawComponents(Object object)
    {
        // Self ���
        if (object.HasComponent<SelfComponent>())
        {
            auto& name = object.GetComponent<SelfComponent>().Name;   // ������

            char buffer[256];                               // ��������� buffer
            memset(buffer, 0, sizeof(buffer));              // �� buffer ����
            strcpy_s(buffer, sizeof(buffer), name.c_str()); // buffer = name

            ImGui::Dummy(ImVec2(0, 0)); // ͸���ؼ�
            ImGui::SameLine();

            ImFont* boldFont = ImGui::GetIO().Fonts->Fonts[0];  // ����
            ImGui::PushFont(boldFont);

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 10));  // ����һ���ؼ��ļ��
            // �����������������ݸı�ʱ
            if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
            {
                name = std::string(buffer); // ���� name ��Ϊ buffer
            }
            ImGui::PopStyleVar();

            ImGui::PopFont();
        }

        // Transform ���
        DrawComponent<TransformComponent>("Transform", object, [](TransformComponent& transformComponent)
        {
            auto& transform = transformComponent.Transform;

            GUI::DragFloat3("Position", transform.GetPosition());   // λ��
            GUI::DragFloat3("Rotation", transform.GetRotation());   // ��ת
            GUI::DragFloat3("Scale", transform.GetScale());         // ����
        });

        // Camera ���
        DrawComponent<CameraComponent>("Camera", object, [](CameraComponent& cameraComponent)
        {
            auto& camera = cameraComponent.Camera;

            GUI::CheckBox("Main Camera", &cameraComponent.Primary); // ��������ÿ�

            const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };                            // ͶӰ���ͣ�͸�� ���� 
            const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];   // ��ǰͶӰ����

            // ������ ѡ��ͶӰ����
            GUI::DropdownList("Projection", currentProjectionTypeString, projectionTypeStrings, 2, [&](int index, const char* value)
            {
                camera.SetProjectionType((SceneCamera::ProjectionType)index);   // �������ͶӰ����
            });

            // ͸��ͶӰ
            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
            {
                GUI::DragSlider("Vertical FOV", &camera.GetFOV_Ref(), 1.0f, 179.0f, GUI::ValueType::Angle); // FOV ��ֱ�Ž� ������
            }

            // ����ͶӰ
            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
            {
                GUI::DragFloatN("Size", &camera.GetSize_Ref());  // �ߴ� �϶���
            }

            GUI::DragFloatN("Near", &camera.GetNearClip_Ref(), 0.01f, GUI::ValueType::Float, 0.01f, camera.GetFarClip() - 0.01f);    // ���ü�ƽ��
            GUI::DragFloatN("Far", &camera.GetFarClip_Ref(), 0.01f, GUI::ValueType::Float, camera.GetNearClip() + 0.01f, 1000.0f);   // Զ�ü�ƽ��
        });

        // SpriteRenderer ���
        DrawComponent<SpriteRendererComponent>("Sprite Renderer", object, [](SpriteRendererComponent& spriteRendererComponent)
        {
            uint32_t spriteID = spriteRendererComponent.Sprite->GetRendererID();    // Sprite ID
            // ѡ��ͼƬ
            GUI::ObjectSelector("Sprite", spriteID, { 50, 50 }, [&]()
            {
                std::string filepath = FileDialogs::OpenFile("Sprite(*.png)\0*.png\0"); // TODO .sprite in project
                
                if (!filepath.empty())
                {
                    spriteRendererComponent.Sprite = Texture2D::Create(filepath);   // ���� Texture
                }
            });

            GUI::ColorEditor4("Color", spriteRendererComponent.Color);   // ��ɫ�༭��
        });

        // ���� Rigidbody2D ���
        DrawComponent<Rigidbody2DComponent>("Rigidbody 2D", object, [](Rigidbody2DComponent& rigidbody2DComponent)
        {
            Rigidbody2D& rigidbody2D = rigidbody2DComponent.Rigidbody2d;

            const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };   // �������ͣ���̬ ��̬ ����ѧ 
            const char* currentBodyTypeString = bodyTypeStrings[(int)rigidbody2D.GetBodyType()];    // ��ǰ��������

            // ������ ��������
            GUI::DropdownList("Body Type", currentBodyTypeString, bodyTypeStrings, 3, [&](int index, const char* value)
            {
                rigidbody2D.SetBodyType((Rigidbody2D::BodyType)index);              // ���ø�������
            });

            GUI::CheckBox("Freeze Rotation", &rigidbody2D.IsFreezeRotation_Ref());  // �Ƿ񶳽���ת Z �� ��ѡ��
        });

        // ���� BoxCollider2D ���
        DrawComponent<BoxCollider2DComponent>("BoxCollider 2D", object, [](BoxCollider2DComponent& boxCollider2DComponent)
        {
            BoxCollider2D& boxCollider2D = boxCollider2DComponent.BoxCollider2d;

            GUI::DragFloatN("Offset", glm::value_ptr(boxCollider2D.GetOffset()), 0.01f, GUI::ValueType::Float2);
            GUI::DragFloatN("Size", glm::value_ptr(boxCollider2D.GetSize()), 0.01f, GUI::ValueType::Float2, 0.0f);

            GUI::DragFloatN("Density", &boxCollider2D.GetDensity_Ref());
            GUI::DragFloatN("Friction", &boxCollider2D.GetFriction_Ref(), 0.01f, GUI::ValueType::Float, 0.0f, 1.0f);
            GUI::DragFloatN("Restitution", &boxCollider2D.GetRestitution_Ref(), 0.01f, GUI::ValueType::Float, 0.0f, 1.0f);
        });

        // ���� CircleCollider2D ���
        DrawComponent<CircleCollider2DComponent>("CircleCollider 2D", object, [](CircleCollider2DComponent& circleCollider2DComponent)
        {
            CircleCollider2D& circleCollider2D = circleCollider2DComponent.CircleCollider2d;

            GUI::DragFloatN("Offset", glm::value_ptr(circleCollider2D.GetOffset()), 0.01f, GUI::ValueType::Float2);
            GUI::DragFloatN("Radius", &circleCollider2D.GetRadius_Ref(), 0.01f, GUI::ValueType::Float2, 0.0f);

            GUI::DragFloatN("Density", &circleCollider2D.GetDensity_Ref());
            GUI::DragFloatN("Friction", &circleCollider2D.GetFriction_Ref(), 0.01f, GUI::ValueType::Float, 0.0f, 1.0f);
            GUI::DragFloatN("Restitution", &circleCollider2D.GetRestitution_Ref(), 0.01f, GUI::ValueType::Float, 0.0f, 1.0f);
        });

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 1));   // ��ֱ���Ϊ 1
        ImGui::Separator(); // �ָ���
        ImGui::PopStyleVar();
        ImGui::Separator(); // �ָ���

        AddComponents(object);      // ������ UI
    }
}