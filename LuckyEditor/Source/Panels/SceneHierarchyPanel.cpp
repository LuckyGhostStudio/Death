#include "SceneHierarchyPanel.h"

#include "Lucky/Scene/Components/SelfComponent.h"

#include <imgui/imgui.h>

namespace Lucky
{
    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
    {
        SetScene(scene);
    }

    void SceneHierarchyPanel::SetScene(const Ref<Scene>& scene)
    {
        m_Scene = scene;
    }

    void SceneHierarchyPanel::OnImGuiRender()
    {
        ImGui::Begin("Hierarchy");
        {
            // ������������ʵ�壬������ each �ڵĺ���
            m_Scene->m_Registry.each([&](auto ObjectID)
            {
                Object object{ ObjectID, m_Scene.get() };

                DrawObjectNode(object); // ����������
            });

            // �����ͣ�ڸô��� && ������ ������հ�λ�ã�
            if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            {
                m_SelectionObject = {}; // ȡ��ѡ�У��ÿ�ѡ������
            }

            // �������� �Ҽ�������ڰ����򵯳��˵���- �Ҽ� ������������
            if (ImGui::BeginPopupContextWindow(0, 1, false))
            {
                // �˵������������
                if (ImGui::MenuItem("Create Empty Object"))
                {
                    m_Scene->CreateObject("Empty Object");  // ����������
                }

                ImGui::EndPopup();
            }
        }
        ImGui::End();
    }

    void SceneHierarchyPanel::DrawObjectNode(Object object)
    {
        auto& name = object.GetComponent<SelfComponent>().ObjectName;   // ������

        // ������־�����ƵĽڵ��Ƿ�ѡ�� ����ѡ�еı�־ ��0 | ������ͷʱ�򿪣�
        ImGuiTreeNodeFlags flags = (m_SelectionObject == object ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        flags |= ImGuiTreeNodeFlags_SpanAvailWidth;	//ˮƽ���쵽�߿�

        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)object, flags, name.c_str());    // ���ڵ㣺��� id ����־ �������ʵ������

        // ����㱻���
        if (ImGui::IsItemClicked())
        {
            m_SelectionObject = object; // ����ѡ������

            LC_TRACE("��ǰѡ�����壺{0} {1}", m_SelectionObject.GetComponent<SelfComponent>().ObjectName, (uint32_t)m_SelectionObject);
        }

        // ɾ������
        bool objectDeleted = false;     // �����Ƿ���ɾ��
        // �Ҽ������������
        if (ImGui::BeginPopupContextItem())
        {
            // �˵��ɾ������
            if (ImGui::MenuItem("Delete Object"))
            {
                objectDeleted = true;   // ������Ϊ��ɾ������Ⱦ��������� UI ��ɾ��������
            }

            ImGui::EndPopup();
        }

        // ������Ѵ�
        if (opened)
        {
            ImGui::TreePop();
        }

        if (objectDeleted)
        {
            m_Scene->DeleteObject(object);      // ɾ������

            // ɾ��������Ϊ��ѡ������
            if (m_SelectionObject == object)
            {
                m_SelectionObject = {};         // �����ѡ������
            }
        }
    }
}