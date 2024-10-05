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
        // 属性面板
        ImGui::Begin("Properties");
        {
            // 被选中的物体存在 TODO 添加资产信息绘制
            if (m_Selection.GetSelection())
            {
                DrawComponents(m_Selection.GetSelection()); // 绘制组件
            }
        }
        ImGui::End();
    }

    void ObjectPropertiesPanel::DrawComponents(Object object)
    {
        // Self 组件存在
        if (object.HasComponent<SelfComponent>())
        {
            auto& name = object.GetComponent<SelfComponent>().ObjectName;   // 物体名

            char buffer[256];                               // 输入框内容 buffer
            memset(buffer, 0, sizeof(buffer));              // 将 buffer 置零
            strcpy_s(buffer, sizeof(buffer), name.c_str()); // buffer = name

            // 创建输入框，输入框内容改变时
            if (ImGui::InputText("Name", buffer, sizeof(buffer)))
            {
                name = std::string(buffer); // 物体 name 设为 buffer
            }
        }

        // Transform 组件存在
        if (object.HasComponent<TransformComponent>())
        {
            // Transform 组件结点：Transform 组件类的哈希值作为结点 id 默认打开
            if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
            {
                auto& transform = object.GetComponent<TransformComponent>();

                ImGui::DragFloat3("Position", glm::value_ptr(transform.Position), 0.1f);    //位置：拖动速度0.1
                ImGui::DragFloat3("Rotation", glm::value_ptr(transform.Rotation), 0.1f);    //旋转：拖动速度0.1
                ImGui::DragFloat3("Scale", glm::value_ptr(transform.Scale), 0.1f);          //缩放：拖动速度0.1

                ImGui::TreePop();
            }
        }
    }
}
