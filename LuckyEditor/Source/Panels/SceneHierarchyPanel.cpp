#include "SceneHierarchyPanel.h"

#include "Lucky/Core/Application.h"
#include "Lucky/Core/Log.h"
#include "Lucky/Core/Input/Input.h"
#include "Lucky/Scene/Components/SelfComponent.h"
#include "Lucky/Scene/Components/SpriteRendererComponent.h"
#include "Lucky/Scene/Components/CameraComponent.h"
#include "Lucky/Scene/Components/Rigidbody2DComponent.h"
#include "Lucky/Scene/Components/BoxCollider2DComponent.h"
#include "Lucky/Scene/Selection.h"

#include <imgui/imgui.h>

namespace Lucky
{
    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
        : m_Scene(scene)
    {

    }

    void SceneHierarchyPanel::SetSceneContext(const Ref<Scene>& scene)
    {
        // 重新设置场景信息
        m_Scene = scene;
        Selection::Object = {};
    }

    void SceneHierarchyPanel::OnImGuiRender(bool& isOpen)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 6)); // 窗口 Padding（控件边界到窗口边界的距离）
        ImGui::Begin("Hierarchy"/*, &isOpen*/);
        {
            ImGui::PopStyleVar();

            m_IsFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 0));   // 设置树节点之间的垂直距离为 0

            if (m_Scene)
            {
                // TODO 添加场景名节点 场景保存状态显示 Name *
                // 遍历场景所有实体，并调用 each 内的函数
                m_Scene->m_Registry.each([&](auto ObjectID)
                {
                    Object object{ ObjectID, m_Scene.get() };

                    DrawObjectNode(object); // 绘制物体结点
                });


                ImGui::PopStyleVar();

                // 点击鼠标 && 鼠标悬停在该窗口（点击空白位置）
                if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered())
                {
                    Selection::Object = {}; // 取消选中：置空选中物体
                }

                // 创建物体 右键点击窗口白区域弹出菜单：- 右键 不在物体项上
                if (ImGui::BeginPopupContextWindow(nullptr, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
                {
                    Object newObject;

                    // 创建空物体
                    if (ImGui::MenuItem("Create Empty"))
                    {
                        m_Scene->CreateObject("Object");
                    }
                    // 创建 Sprite
                    if (ImGui::MenuItem("Sprite"))
                    {
                        newObject = m_Scene->CreateObject("Sprite");
                        newObject.AddComponent<SpriteRendererComponent>();
                    }
                    // 创建 Camera
                    if (ImGui::MenuItem("Camera"))
                    {
                        newObject = m_Scene->CreateObject("Camera");
                        newObject.AddComponent<CameraComponent>();
                    }
                    // 创建 2D 刚体 TODO Test
                    if (ImGui::MenuItem("Rigidbody 2D"))
                    {
                        newObject = m_Scene->CreateObject("Rigidbody 2D");
                        newObject.AddComponent<SpriteRendererComponent>();
                        newObject.AddComponent<Rigidbody2DComponent>();
                        newObject.AddComponent<BoxCollider2DComponent>();
                    }

                    ImGui::EndPopup();
                }
            }
        }
        ImGui::End();
    }

    void SceneHierarchyPanel::DrawObjectNode(Object object)
    {
        std::string& name = object.GetComponent<SelfComponent>().Name;  // 物体名

        // 树结点标志
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        
        static ImVec4 headerColor = { 0.38f, 0.3805f, 0.381f, 1.0f }; // 树节点颜色

        if (Selection::Object == object)
        {
            flags |= ImGuiTreeNodeFlags_Selected;

            headerColor = { 0.2588f, 0.5882f, 0.9804f, 0.3490f };   // 设置选中颜色
        }
        else
        {
            headerColor = { 0.38f, 0.3805f, 0.381f, 1.0f };
        }

        ImGui::PushStyleColor(ImGuiCol_Header, headerColor);
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, headerColor);
        ImGui::PushStyleColor(ImGuiCol_HeaderActive, headerColor);

        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)object, flags, name.c_str());    // 树节点：结点 id 结点标志 结点名（实体名）
        
        ImGui::PopStyleColor(3);

        // 树结点被点击
        if (ImGui::IsItemClicked())
        {
            Selection::Object = object; // 设置选中物体

            LC_TRACE("Selected Object：[ENTT = {0}, UUID {1}, Name {2}]", (uint32_t)Selection::Object, Selection::Object.GetUUID(), Selection::Object.GetName());
        }

        // 删除物体
        bool objectDeleted = false;     // 物体是否已删除
        // 右键点击该物体结点
        if (ImGui::BeginPopupContextItem())
        {
            // 菜单项：删除物体
            if (ImGui::MenuItem("Delete"))
            {
                objectDeleted = true;   // 物体标记为已删除：渲染结束后面的 UI 再删除该物体
            }

            ImGui::EndPopup();
        }

        // 树结点已打开
        if (opened)
        {
            // TODO 子节点

            ImGui::TreePop();
        }

        if (objectDeleted)
        {
            m_Scene->DeleteObject(object);  // 删除物体

            // 删除的物体为已选中物体
            if (Selection::Object == object)
            {
                Selection::Object = {};     // 清空已选中物体
            }
        }
    }

    void SceneHierarchyPanel::OnDuplicateObject()
    {
        if (Selection::Object)
        {
            m_Scene->DuplicateObject(Selection::Object);    // 复制当前选中物体
        }
    }

    void SceneHierarchyPanel::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<KeyPressedEvent>(LC_BIND_EVENT_FUNC(SceneHierarchyPanel::OnKeyPressed));    // 调用按键按下事件
    }

    bool SceneHierarchyPanel::OnKeyPressed(KeyPressedEvent& e)
    {
        // 快捷键：重复次数 == 0
        if (e.GetRepeatCount() > 0)
        {
            return false;
        }

        bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl); // Ctrl 键按下
        bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);       // Shift 键按下

        switch (e.GetKeyCode())
        {
            case Key::D:
                if (control)
                {
                    OnDuplicateObject();    // Ctrl+D 复制物体
                }
                break;
        }

        return false;
    }
}