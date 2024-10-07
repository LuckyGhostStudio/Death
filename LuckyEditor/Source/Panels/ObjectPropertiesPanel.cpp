#include "ObjectPropertiesPanel.h"

#include "Lucky/Scene/Components/SelfComponent.h"
#include "Lucky/Scene/Components/TransformComponent.h"
#include "Lucky/Scene/Components/CameraComponent.h"
#include "Lucky/Scene/Components/SpriteRendererComponent.h"

#include "Lucky/Scene/Selection.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Lucky
{
    void ObjectPropertiesPanel::OnImGuiRender()
    {
        // 属性面板
        ImGui::Begin("Properties");
        {
            // 被选中的物体存在 TODO 添加资产信息绘制
            if (Selection::Object)
            {
                DrawComponents(Selection::Object);  // 绘制组件 UI
            }
        }
        ImGui::End();
    }

    void ObjectPropertiesPanel::AddComponents(Object object)
    {
        // 添加组件
        if (ImGui::Button("Add Component"))
        {
            ImGui::OpenPopup("AddComponent");   // 打开弹出框
        }

        // 渲染弹出框
        if (ImGui::BeginPopup("AddComponent"))
        {
            // 添加 Camera 组件
            if (ImGui::MenuItem("Camera"))
            {
                object.AddComponent<CameraComponent>();

                ImGui::CloseCurrentPopup();
            }

            // 添加 SpriteRenderer 组件
            if (ImGui::MenuItem("Sprite Renderer"))
            {
                object.AddComponent<SpriteRendererComponent>();

                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void ObjectPropertiesPanel::DrawComponents(Object object)
    {
        AddComponents(object);  // 添加组件 UI

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

        // 树节点标志：打开|允许重叠
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

        // Transform 组件
        if (object.HasComponent<TransformComponent>())
        {
            // Transform 组件结点：Transform 组件类的哈希值作为结点 id
            if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treeNodeFlags, "Transform"))
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
            if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), treeNodeFlags, "Camera"))
            {
                auto& camera= object.GetComponent<CameraComponent>().Camera;

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

        // SpriteRenderer组件
        if (object.HasComponent<SpriteRendererComponent>())
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));  // 设置边框样式

            // SpriteRenderer 组件结点
            bool opened = ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), treeNodeFlags, "Sprite Renderer");

            ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);   // 同一行：获取窗口宽度
                
            // 组件设置按钮
            if (ImGui::Button("+", ImVec2(20, 20)))
            {
                ImGui::OpenPopup("ComponentSettings");  // 打开弹出框
            }

            ImGui::PopStyleVar();

            // 移除组件
            bool componentRemoved = false;
            // 渲染弹出框
            if (ImGui::BeginPopup("ComponentSettings"))
            {
                // 移除组件菜单项
                if (ImGui::MenuItem("Remove Component"))
                {
                    componentRemoved = true;    // 组件标记为移除
                }

                ImGui::EndPopup();
            }

            if (opened)
            {
                auto& spriteRenderer = object.GetComponent<SpriteRendererComponent>();

                ImGui::ColorEdit4("Color", glm::value_ptr(spriteRenderer.Color));   // 颜色编辑器

                ImGui::TreePop();
            }

            if (componentRemoved)
            {
                object.RemoveComponent<SpriteRendererComponent>();  // 移除 SpriteRenderer 组件
            }
        }
    }
}
