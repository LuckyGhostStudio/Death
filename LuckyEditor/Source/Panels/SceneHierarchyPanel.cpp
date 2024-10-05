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
            Object Object{ ObjectID, m_Scene.get() };

            DrawObjectNode(Object);     // ����ʵ����
        });

        ImGui::End();
    }

    void SceneHierarchyPanel::DrawObjectNode(Object Object)
    {
        auto& name = Object.GetComponent<SelfComponent>().ObjectName;   // ������

        // ������־�����ƵĽڵ��Ƿ�ѡ�� ����ѡ�еı�־ ��0 | ������ͷʱ�򿪣�
        ImGuiTreeNodeFlags flags = (m_SelectionObject == Object ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)Object, flags, name.c_str());    // ���ڵ㣺��� id ����־ �������ʵ������

        // ����㱻���
        if (ImGui::IsItemClicked())
        {
            m_SelectionObject = Object; // Object ��ѡ��
        }

        // ������Ѵ�
        if (opened)
        {
            ImGui::TreePop();   // չ�����
        }
    }
}