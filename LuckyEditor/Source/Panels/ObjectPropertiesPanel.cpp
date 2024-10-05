#include "ObjectPropertiesPanel.h"

#include "Lucky/Scene/Components/SelfComponent.h"
#include "Lucky/Scene/Components/TransformComponent.h"
#include "Lucky/Scene/Components/CameraComponent.h"

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
        // Self 组件
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

        // Transform 组件
        if (object.HasComponent<TransformComponent>())
        {
            // Transform 组件结点：Transform 组件类的哈希值作为结点 id 默认打开
            if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
            {
                auto& transform = object.GetComponent<TransformComponent>().Transform;

                ImGui::DragFloat3("Position", glm::value_ptr(transform.GetPosition()), 0.1f);    //位置：拖动速度0.1
                ImGui::DragFloat3("Rotation", glm::value_ptr(transform.GetRotation()), 0.1f);    //旋转：拖动速度0.1
                ImGui::DragFloat3("Scale", glm::value_ptr(transform.GetScale()), 0.1f);          //缩放：拖动速度0.1

                ImGui::TreePop();
            }
        }

        // Camera 组件
        if (object.HasComponent<CameraComponent>())
        {
            // Camera 组件结点
            if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera")) 
            {
                auto& cameraComponent = object.GetComponent<CameraComponent>();
                SceneCamera& camera = cameraComponent.Camera;

                ImGui::Checkbox("Main Camera", &camera.IsPrimary_Ref());    // 主相机设置框

                const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };                            // 投影类型：透视 正交 
                const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];   // 当前投影类型

                // 下拉框 选择投影类型
                if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
                {
                    for (int i = 0; i < 2; i++)
                    {
                        bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];  // 被选中：当前投影类型==第i个投影类型
                        
                        // 可选择项，该项改变时：投影类型 已选中
                        if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
                        {
                            currentProjectionTypeString = projectionTypeStrings[i];     // 设置当前投影类型
                            camera.SetProjectionType((SceneCamera::ProjectionType)i);   // 设置相机投影类型
                        }
                        if (isSelected)
                        {
                            ImGui::SetItemDefaultFocus();   // 设置默认选中项
                        }
                    }
                    ImGui::EndCombo();
                }

                // 透视投影
                if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
                {
                    float verticalFov = camera.GetFOV();    // 垂直张角

                    if (ImGui::SliderAngle("Vertical Fov", &verticalFov))
                    {
                        camera.SetFOV(verticalFov);
                    }
                }

                // 正交投影
                if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
                {
                    float size = camera.GetSize();          // 尺寸

                    if (ImGui::DragFloat("Size", &size))
                    {
                        camera.SetSize(size);
                    }
                }

                float nearClip = camera.GetNearClip();  // 近裁剪平面

                if (ImGui::DragFloat("Near", &nearClip))
                {
                    camera.SetNearClip(nearClip);
                }

                float farClip = camera.GetFarClip();    // 远裁剪平面

                if (ImGui::DragFloat("Far", &farClip))
                {
                    camera.SetFarClip(farClip);
                }

                ImGui::TreePop();
            }
        }
    }
}
