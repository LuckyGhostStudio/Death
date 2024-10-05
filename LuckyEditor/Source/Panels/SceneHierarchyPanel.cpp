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

        // 遍历场景所有实体，并调用 each 内的函数
        m_Scene->m_Registry.each([&](auto ObjectID)
        {
            Object Object{ ObjectID, m_Scene.get() };

            DrawObjectNode(Object);     // 绘制实体结点
        });

        ImGui::End();
    }

    void SceneHierarchyPanel::DrawObjectNode(Object Object)
    {
        auto& name = Object.GetComponent<SelfComponent>().ObjectName;   // 物体名

        // 树结点标志（绘制的节点是否被选中 ？被选中的标志 ：0 | 单击箭头时打开）
        ImGuiTreeNodeFlags flags = (m_SelectionObject == Object ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)Object, flags, name.c_str());    // 树节点：结点 id 结点标志 结点名（实体名）

        // 树结点被点击
        if (ImGui::IsItemClicked())
        {
            m_SelectionObject = Object; // Object 被选中
        }

        // 树结点已打开
        if (opened)
        {
            ImGui::TreePop();   // 展开结点
        }
    }
}