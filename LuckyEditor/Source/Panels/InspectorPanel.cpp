#include "InspectorPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

#include "Lucky/Scene/Components/SelfComponent.h"
#include "Lucky/Scene/Components/TransformComponent.h"
#include "Lucky/Scene/Components/CameraComponent.h"
#include "Lucky/Scene/Components/SpriteRendererComponent.h"

#include "Lucky/ImGui/GUI.h"

#include "Lucky/Utils/PlatformUtils.h"

namespace Lucky
{
    void InspectorPanel::OnImGuiRender(bool& isOpen)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 10));    // 窗口 Padding（控件边界到窗口边界的距离）
        ImGui::Begin("Inspector"/*, &isOpen*/);
        {
            ImGui::PopStyleVar();

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 8));   // 垂直间距为 4

            // 被选中的物体存在 TODO 添加资产信息绘制
            if (m_SelectionObject)
            {
                DrawComponents(m_SelectionObject);  // 绘制组件 UI
            }

            ImGui::PopStyleVar();
        }
        ImGui::End();
    }

    void InspectorPanel::AddComponents(Object object)
    {
        float panelWidth = ImGui::GetWindowContentRegionWidth() + ImGui::GetStyle().WindowPadding.x * 2;    // 面板宽度

        // 计算居中的位置
        float posX = (panelWidth - 300) * 0.5f;

        // 设置按钮的位置
        ImGui::SetCursorPosX(posX);

        // 添加组件
        if (ImGui::Button("Add Component", ImVec2(300, 0)))
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
        const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth;

        // T 组件存在
        if (object.HasComponent<T>())
        {
            auto& component = object.GetComponent<T>();    // 获得组件

            ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();     // 可用区域大小
            
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 1));   // Separator 和 TreeNodeEx 之间的垂直距离为 1
            ImGui::Separator(); // 分隔符
            ImGui::Separator(); // 分隔符

            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);            // 边框圆度
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);          // 边框线宽 0
            
            float lineHeight = GImGui->Font->FontSize; // +GImGui->Style.FramePadding.y * 2.0f;    // 行高 = 字体大小 + 边框 y * 2
            float padding = GImGui->Style.FramePadding.y;

            ImFont* boldFont = ImGui::GetIO().Fonts->Fonts[0];  // 粗体
            ImGui::PushFont(boldFont);

            //ImVec2 nodePos = ImGui::GetCursorPos(); // 目录结点位置坐标

            // 组件结点：组件类的哈希值作为结点 id
            bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, name.c_str());

            ImGui::PopFont();

            static Ref<Texture2D> settingsButtonIcon = Texture2D::Create("Resources/Icons/SettingsButton_Icon.png");

            ImGui::SameLine(contentRegionAvail.x - lineHeight - padding);      // 同一行：可用区域左移
            ImGui::SetCursorPosY(ImGui::GetCursorPos().y + padding);
            // 组件设置按钮
            if (ImGui::ImageButton((void*)settingsButtonIcon->GetRendererID(), { lineHeight - padding * 2, lineHeight - padding * 2 }, ImVec2(0, 1), ImVec2(1, 0)))
            {
                ImGui::OpenPopup("ComponentSettings");  // 打开弹出框
            }

            ImGui::PopStyleVar(4);

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
                ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
                ImGui::SetCursorPosY(ImGui::GetCursorPos().y - padding);
                ImGui::Separator(); // 分隔符
                ImGui::PopStyleColor();

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

            ImGui::Dummy(ImVec2(0, 0)); // 透明控件
            ImGui::SameLine();

            ImFont* boldFont = ImGui::GetIO().Fonts->Fonts[0];  // 粗体
            ImGui::PushFont(boldFont);

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 10));  // 和下一个控件的间距
            // 创建输入框，输入框内容改变时
            if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
            {
                name = std::string(buffer); // 物体 name 设为 buffer
            }
            ImGui::PopStyleVar();

            ImGui::PopFont();
        }

        // Transform 组件
        DrawComponent<TransformComponent>("Transform", object, [](TransformComponent& transformComponent)
        {
            auto& transform = transformComponent.Transform;

            GUI::DragFloat3("Position", transform.GetPosition());   // 位置
            GUI::DragFloat3("Rotation", transform.GetRotation());   // 旋转
            GUI::DragFloat3("Scale", transform.GetScale());         // 缩放
        });

        // Camera 组件
        DrawComponent<CameraComponent>("Camera", object, [](CameraComponent& cameraComponent)
        {
            auto& camera = cameraComponent.Camera;

            GUI::CheckBox("Main Camera", &camera.IsPrimary_Ref());    // 主相机设置框

            const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };                            // 投影类型：透视 正交 
            const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];   // 当前投影类型

            // 下拉框 选择投影类型
            GUI::DropdownList("Projection", currentProjectionTypeString, projectionTypeStrings, 2, [&](int index, const char* value)
            {
                camera.SetProjectionType((SceneCamera::ProjectionType)index);   // 设置相机投影类型
            });

            // 透视投影
            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
            {
                GUI::DragSlider("Vertical FOV", &camera.GetFOV_Ref(), 1.0f, 179.0f, GUI::ValueType::Angle); // FOV 垂直张角 滑动条
            }

            // 正交投影
            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
            {
                GUI::DragFloatN("Size", &camera.GetSize_Ref());  // 尺寸 拖动条
            }

            GUI::DragFloatN("Near", &camera.GetNearClip_Ref(), 0.01f, GUI::ValueType::Float, 0.01f, camera.GetFarClip() - 0.01f);    // 近裁剪平面
            GUI::DragFloatN("Far", &camera.GetFarClip_Ref(), 0.01f, GUI::ValueType::Float, camera.GetNearClip() + 0.01f, 1000.0f);   // 远裁剪平面
        });

        // SpriteRenderer 组件
        DrawComponent<SpriteRendererComponent>("Sprite Renderer", object, [](SpriteRendererComponent& spriteRendererComponent)
        {
            uint32_t spriteID = spriteRendererComponent.Sprite->GetRendererID();    // Sprite ID
            // 选择图片
            GUI::ObjectSelector("Sprite", spriteID, { 50, 50 }, [&]()
            {
                std::string filepath = FileDialogs::OpenFile("Sprite(*.png)\0*.png\0"); // TODO .sprite in project
                
                if (!filepath.empty())
                {
                    spriteRendererComponent.Sprite = Texture2D::Create(filepath);   // 创建 Texture
                }
            });

            GUI::ColorEditor4("Color", spriteRendererComponent.Color);   // 颜色编辑器
        });

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 1));   // 垂直间距为 1
        ImGui::Separator(); // 分隔线
        ImGui::PopStyleVar();
        ImGui::Separator(); // 分隔线

        AddComponents(object);      // 添加组件 UI
    }
}