#include "InspectorPanel.h"

#include "Lucky/Scene/Components/SelfComponent.h"
#include "Lucky/Scene/Components/TransformComponent.h"
#include "Lucky/Scene/Components/CameraComponent.h"
#include "Lucky/Scene/Components/SpriteRendererComponent.h"

#include "Lucky/ImGui/GUI.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

namespace Lucky
{
    InspectorPanel::InspectorPanel()
        : EditorWindow("Inspector")
    {
    
    }

    InspectorPanel::~InspectorPanel()
    {

    }

    void InspectorPanel::OnUpdate(DeltaTime dt)
    {

    }

    void InspectorPanel::OnImGuiRender()
    {
        ImGui::Begin(m_Name.c_str());
        {
            // 被选中的物体存在 TODO 添加资产信息绘制
            if (m_SelectionObject)
            {
                DrawComponents(m_SelectionObject);  // 绘制组件 UI
            }
        }
        ImGui::End();
    }

    void InspectorPanel::OnEvent(Event& e)
    {

    }

    void InspectorPanel::AddComponents(Object object)
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

    void InspectorPanel::DrawComponents(Object object)
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

            GUI::DragVector3("Position", transform.GetPosition());  // 位置：默认值 1.0f
            GUI::DragVector3("Rotation", transform.GetRotation());  // 旋转：默认值 1.0f
            GUI::DragVector3("Scale", transform.GetScale(), 1.0f);  // 缩放：默认值 1.0f
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
                GUI::DragSlider("Vertical FOV", &camera.GetFOV_Ref(), 1.0f, 179.0f, GUI::ValueType::Angle); // FOV 垂直张角 滑动条
            }

            // 正交投影
            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
            {
                GUI::DragFloat("Size", &camera.GetSize_Ref());  // 尺寸 拖动条
            }

            GUI::DragFloat("Near", &camera.GetNearClip_Ref(), 0.01f, GUI::ValueType::Float, 0.01f, camera.GetFarClip() - 0.01f);    // 近裁剪平面
            GUI::DragFloat("Far", &camera.GetFarClip_Ref(), 0.01f, GUI::ValueType::Float, camera.GetNearClip() + 0.01f, 1000.0f);   // 远裁剪平面
        });

        // SpriteRenderer 组件
        DrawComponent<SpriteRendererComponent>("Sprite Renderer", object, [](SpriteRendererComponent& spriteRendererComponent)
        {
            ImGui::ColorEdit4("Color", glm::value_ptr(spriteRendererComponent.Color));   // 颜色编辑器
        });
    }
}