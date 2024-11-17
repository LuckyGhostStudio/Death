#include "InspectorPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

#include "Lucky/Scene/Components/SelfComponent.h"
#include "Lucky/Scene/Components/TransformComponent.h"
#include "Lucky/Scene/Components/CameraComponent.h"
#include "Lucky/Scene/Components/SpriteRendererComponent.h"

#include "Lucky/ImGui/GUI.h"

#include "Lucky/Utils/PlatformUtils.h"

namespace Lucky
{
    void InspectorPanel::OnImGuiRender(bool& isOpen)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 10));    // ���� Padding���ؼ��߽絽���ڱ߽�ľ��룩
        ImGui::Begin("Inspector"/*, &isOpen*/);
        {
            ImGui::PopStyleVar();

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
        const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth;

        // T �������
        if (object.HasComponent<T>())
        {
            auto& component = object.GetComponent<T>();    // ������

            ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();     // ���������С
            
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 1));   // Separator �� TreeNodeEx ֮��Ĵ�ֱ����Ϊ 1
            ImGui::Separator(); // �ָ���
            ImGui::Separator(); // �ָ���

            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);            // �߿�Բ��
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);          // �߿��߿� 0
            
            float lineHeight = GImGui->Font->FontSize; // +GImGui->Style.FramePadding.y * 2.0f;    // �и� = �����С + �߿� y * 2
            float padding = GImGui->Style.FramePadding.y;

            ImFont* boldFont = ImGui::GetIO().Fonts->Fonts[0];  // ����
            ImGui::PushFont(boldFont);

            //ImVec2 nodePos = ImGui::GetCursorPos(); // Ŀ¼���λ������

            // �����㣺�����Ĺ�ϣֵ��Ϊ��� id
            bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, name.c_str());

            ImGui::PopFont();

            static Ref<Texture2D> settingsButtonIcon = Texture2D::Create("Resources/Icons/SettingsButton_Icon.png");

            ImGui::SameLine(contentRegionAvail.x - lineHeight - padding);      // ͬһ�У�������������
            ImGui::SetCursorPosY(ImGui::GetCursorPos().y + padding);
            // ������ð�ť
            if (ImGui::ImageButton((void*)settingsButtonIcon->GetRendererID(), { lineHeight - padding * 2, lineHeight - padding * 2 }, ImVec2(0, 1), ImVec2(1, 0)))
            {
                ImGui::OpenPopup("ComponentSettings");  // �򿪵�����
            }

            ImGui::PopStyleVar(4);

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
                ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
                ImGui::SetCursorPosY(ImGui::GetCursorPos().y - padding);
                ImGui::Separator(); // �ָ���
                ImGui::PopStyleColor();

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

            GUI::CheckBox("Main Camera", &camera.IsPrimary_Ref());    // ��������ÿ�

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

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 1));   // ��ֱ���Ϊ 1
        ImGui::Separator(); // �ָ���
        ImGui::PopStyleVar();
        ImGui::Separator(); // �ָ���

        AddComponents(object);      // ������ UI
    }
}