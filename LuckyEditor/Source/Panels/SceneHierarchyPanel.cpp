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
            // 遍历场景所有实体，并调用 each 内的函数
            m_Scene->m_Registry.each([&](auto ObjectID)
            {
                Object object{ ObjectID, m_Scene.get() };

                DrawObjectNode(object); // 绘制物体结点
            });

            // 鼠标悬停在该窗口 && 点击鼠标 （点击空白位置）
            if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            {
                m_SelectionObject = {}; // 取消选中：置空选中物体
            }

            // 创建物体 右键点击窗口白区域弹出菜单：- 右键 不在物体项上
            if (ImGui::BeginPopupContextWindow(0, 1, false))
            {
                // 菜单项：创建空物体
                if (ImGui::MenuItem("Create Empty Object"))
                {
                    m_Scene->CreateObject("Empty Object");  // 创建空物体
                }

                ImGui::EndPopup();
            }
        }
        ImGui::End();
    }

    void SceneHierarchyPanel::DrawObjectNode(Object object)
    {
        auto& name = object.GetComponent<SelfComponent>().ObjectName;   // 物体名

        // 树结点标志（绘制的节点是否被选中 ？被选中的标志 ：0 | 单击箭头时打开）
        ImGuiTreeNodeFlags flags = (m_SelectionObject == object ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        flags |= ImGuiTreeNodeFlags_SpanAvailWidth;	//水平延伸到边框

        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)object, flags, name.c_str());    // 树节点：结点 id 结点标志 结点名（实体名）

        // 树结点被点击
        if (ImGui::IsItemClicked())
        {
            m_SelectionObject = object; // 设置选中物体

            LC_TRACE("当前选中物体：{0} {1}", m_SelectionObject.GetComponent<SelfComponent>().ObjectName, (uint32_t)m_SelectionObject);
        }

        // 删除物体
        bool objectDeleted = false;     // 物体是否已删除
        // 右键点击该物体结点
        if (ImGui::BeginPopupContextItem())
        {
            // 菜单项：删除物体
            if (ImGui::MenuItem("Delete Object"))
            {
                objectDeleted = true;   // 物体标记为已删除：渲染结束后面的 UI 再删除该物体
            }

            ImGui::EndPopup();
        }

        // 树结点已打开
        if (opened)
        {
            ImGui::TreePop();
        }

        if (objectDeleted)
        {
            m_Scene->DeleteObject(object);      // 删除物体

            // 删除的物体为已选中物体
            if (m_SelectionObject == object)
            {
                m_SelectionObject = {};         // 清空已选中物体
            }
        }
    }
}