#include "ObjectPropertiesPanel.h"

#include "Lucky/Scene/Components/SelfComponent.h"
#include "Lucky/Scene/Components/TransformComponent.h"

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
        // Self �������
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

        // Transform �������
        if (object.HasComponent<TransformComponent>())
        {
            // Transform �����㣺Transform �����Ĺ�ϣֵ��Ϊ��� id Ĭ�ϴ�
            if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
            {
                auto& transform = object.GetComponent<TransformComponent>();

                ImGui::DragFloat3("Position", glm::value_ptr(transform.Position), 0.1f);    //λ�ã��϶��ٶ�0.1
                ImGui::DragFloat3("Rotation", glm::value_ptr(transform.Rotation), 0.1f);    //��ת���϶��ٶ�0.1
                ImGui::DragFloat3("Scale", glm::value_ptr(transform.Scale), 0.1f);          //���ţ��϶��ٶ�0.1

                ImGui::TreePop();
            }
        }
    }
}
