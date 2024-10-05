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

        // ������������ʵ�壬������ each �ڵĺ���
        m_Scene->m_Registry.each([&](auto ObjectID)
        {
            Object object{ ObjectID, m_Scene.get() };

            DrawObjectNode(object);     // ����������
        });

        ImGui::End();
    }

    void SceneHierarchyPanel::DrawObjectNode(Object object)
    {
        auto& name = object.GetComponent<SelfComponent>().ObjectName;   // ������

        // ������־�����ƵĽڵ��Ƿ�ѡ�� ����ѡ�еı�־ ��0 | ������ͷʱ�򿪣�
        ImGuiTreeNodeFlags flags = (m_SelectionObject == object ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)object, flags, name.c_str());    // ���ڵ㣺��� id ����־ �������ʵ������

        // ����㱻���
        if (ImGui::IsItemClicked())
        {
            m_SelectionObject = object; // ����ѡ������

            LC_TRACE("��ǰѡ�����壺{0}", m_SelectionObject.GetComponent<SelfComponent>().ObjectName);
        }

        // ������Ѵ�
        if (opened)
        {
            ImGui::TreePop();
        }
    }
}