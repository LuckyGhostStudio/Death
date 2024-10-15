#include "ObjectPropertiesPanel.h"

#include "Lucky/Scene/Components/SelfComponent.h"
#include "Lucky/Scene/Components/TransformComponent.h"
#include "Lucky/Scene/Components/CameraComponent.h"
#include "Lucky/Scene/Components/SpriteRendererComponent.h"

#include "Lucky/Scene/Selection.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
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

    /// <summary>
    /// 绘制Vector3控件
    /// </summary>
    /// <param name="label">标签</param>
    /// <param name="values">值</param>
    /// <param name="resetValue">重置值</param>
    /// <param name="columnWidth">每列宽度</param>
    static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
    {
        ImGuiIO& io = ImGui::GetIO();
        auto boldFont = io.Fonts->Fonts[0]; // 粗体：0号字体

        ImGui::PushID(label.c_str());   // 设置控件标签

        // 标签列
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);  // 设置 0 号列宽
        ImGui::Text(label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());         // 设置 3 个列宽
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 }); // Var 样式

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;    // 行高 = 字体大小 + 边框.y * 2
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };  // 重置值按钮大小

        // X 分量 UI
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));        // 按钮颜色
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.3f, 0.35f, 1.0f)); // 鼠标悬停在按钮时的颜色
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.1f, 0.15f, 1.0f));  // 按钮按下颜色

        ImGui::PushFont(boldFont);  // 设置粗体

        // X 按钮按下
        if (ImGui::Button("X", buttonSize))
        {
            values.x = resetValue;  // 重置 x 分量
        }

        ImGui::PopFont();

        ImGui::PopStyleColor(3);

        ImGui::SameLine();                          // 在同一行
        ImGui::DragFloat("##X", &values.x, 0.1f);   // X 分量列 ##不显示标签 拖动精度 0.1
        ImGui::PopItemWidth();                      // 推出第一个列宽
        ImGui::SameLine();

        // Y 分量 UI
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));         // 按钮颜色
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.9f, 0.4f, 1.0f));  // 鼠标悬停在按钮时的颜色
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));   // 按钮按下颜色

        ImGui::PushFont(boldFont);  // 设置粗体

        // Y 按钮按下
        if (ImGui::Button("Y", buttonSize))
        {
            values.y = resetValue;  // 重置 y 分量
        }

        ImGui::PopFont();

        ImGui::PopStyleColor(3);

        ImGui::SameLine();                          // 在同一行
        ImGui::DragFloat("##Y", &values.y, 0.1f);   // Y 分量列 ##不显示标签 拖动精度 0.1
        ImGui::PopItemWidth();                      // 推出第一个列宽
        ImGui::SameLine();

        // Z 分量 UI
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.25f, 0.8f, 1.0f));        // 按钮颜色
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.4f, 0.8f, 1.0f));  // 鼠标悬停在按钮时的颜色
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35f, 0.25f, 0.9f, 1.0f)); // 按钮按下颜色

        ImGui::PushFont(boldFont);  // 设置粗体

        // Z 按钮按下
        if (ImGui::Button("Z", buttonSize))
        {
            values.z = resetValue;  // 重置 z 分量
        }

        ImGui::PopFont();

        ImGui::PopStyleColor(3);

        ImGui::SameLine();                          // 在同一行
        ImGui::DragFloat("##Z", &values.z, 0.1f);   // Z 分量列 ##不显示标签 拖动精度 0.1
        ImGui::PopItemWidth();                      // 推出第一个列宽

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();     // 弹出控件标签
    }

    /// <summary>
    /// 绘制组件
    /// </summary>
    /// <typeparam name="T">组件类型</typeparam>
    /// <typeparam name="UIFunction">组件功能函数类型</typeparam>
    /// <param name="name">组件名</param>
    /// <param name="object">实体</param>
    /// <param name="uiFunction">组件功能函数</param>
    template<typename T, typename UIFunction>
    static void DrawComponent(const std::string& name, Object object, UIFunction uiFunction)
    {
        // 树节点标志：打开|框架|延伸到右边|允许重叠|框架边框
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

        // T 组件存在
        if (object.HasComponent<T>())
        {
            auto& component = object.GetComponent<T>();    // 获得组件

            ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();     // 可用区域大小

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));  // 设置边框样式
            float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;    //行高 = 字体大小 + 边框 y * 2
            ImGui::Separator();     // 分隔符

            // 组件结点：组件类的哈希值作为结点 id
            bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
            ImGui::PopStyleVar();

            ImGui::SameLine(contentRegionAvail.x - lineHeight * 0.5f);      // 同一行：可用区域左移

            // 组件设置按钮
            if (ImGui::Button("+", ImVec2(lineHeight, lineHeight)))
            {
                ImGui::OpenPopup("ComponentSettings");  // 打开弹出框
            }

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
                uiFunction(component);  // 调用组件功能函数：绘制该组件不同的部分

                ImGui::TreePop();       // 展开结点
            }

            if (componentRemoved)
            {
                object.RemoveComponent<T>();    // 移除 T 组件
            }
        }
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
            if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
            {
                name = std::string(buffer); // 物体 name 设为 buffer
            }
        }

        ImGui::SameLine();
        ImGui::PushItemWidth(-1);   // 沿着右边框宽-1

        AddComponents(object);      // 添加组件 UI

        // 树节点标志：打开|允许重叠
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

        // Transform 组件
        DrawComponent<TransformComponent>("Transform", object, [](TransformComponent& transformComponent)
        {
            auto& transform = transformComponent.Transform;

            DrawVec3Control("Position", transform.GetPosition());   // 位置：默认值 1.0f
            DrawVec3Control("Rotation", transform.GetRotation());   // 旋转：默认值 1.0f
            DrawVec3Control("Scale", transform.GetScale(), 1.0f);   // 缩放：默认值 1.0f
        });

        // Camera 组件
        DrawComponent<CameraComponent>("Camera", object, [](CameraComponent& cameraComponent)
        {
            auto& camera = cameraComponent.Camera;

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
                float size = camera.GetSize();      // 尺寸

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
        });

        // SpriteRenderer 组件
        DrawComponent<SpriteRendererComponent>("SpriteRenderer", object, [](SpriteRendererComponent& spriteRendererComponent)
        {
            ImGui::ColorEdit4("Color", glm::value_ptr(spriteRendererComponent.Color));   // 颜色编辑器
        });
    }
}