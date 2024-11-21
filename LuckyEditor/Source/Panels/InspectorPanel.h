#pragma once

#include "EditorPanel.h"

#include "Lucky/Scene/Object.h"
#include "Lucky/Scene/Selection.h"

#include "Lucky/Renderer/Texture.h"
#include "Lucky/Scene/Components/Component.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Lucky
{
    /// <summary>
    /// 检视面板：显示选中物体或资产的信息
    /// </summary>
    class InspectorPanel : public EditorPanel
    {
    private:
        Object& m_SelectionObject = Selection::Object;  // 当前选中项

        Ref<Texture2D> m_SettingsButtonIcon;    // 组件设置按钮 Icon
        Ref<Texture2D> m_ComponentIcon;
        Ref<Texture2D> m_TransformIcon;
        Ref<Texture2D> m_CameraIcon;
        Ref<Texture2D> m_SpriteRendererIcon;
    private:
        /// <summary>
        /// 添加组件 UI
        /// </summary>
        /// <param name="object"></param>
        void AddComponents(Object object);

        /// <summary>
        /// 绘制物体的所有组件 UI
        /// </summary>
        /// <param name="object">物体</param>
        void DrawComponents(Object object);
    public:
        InspectorPanel();

        virtual void OnImGuiRender(bool& isOpen) override;
    private:
        /// <summary>
        /// 绘制组件
        /// </summary>
        /// <typeparam name="T">组件类型</typeparam>
        /// <typeparam name="UIFunction">组件功能函数类型</typeparam>
        /// <param name="name">组件名</param>
        /// <param name="object">实体</param>
        /// <param name="uiFunction">组件功能函数</param>
        template<typename T, typename UIFunction>
        void DrawComponent(const std::string& name, Object object, UIFunction uiFunction);
    };

    template<typename T, typename UIFunction>
    inline void InspectorPanel::DrawComponent(const std::string& name, Object object, UIFunction uiFunction)
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
            //ImGui::SetCursorPosY(ImGui::GetCursorPos().y - 8);
            ImGui::Separator(); // 分隔符

            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);            // 边框圆度
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);          // 边框线宽 0

            float lineHeight = GImGui->Font->FontSize; // +GImGui->Style.FramePadding.y * 2.0f;    // 行高 = 字体大小 + 边框 y * 2
            float padding = GImGui->Style.FramePadding.y;

            ImFont* boldFont = ImGui::GetIO().Fonts->Fonts[0];  // 粗体

            ImVec2 nodePos = ImGui::GetCursorPos();

            // 组件结点：组件类的哈希值作为结点 id
            bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, "");

            uint32_t iconID = 0;
            switch (component.Name)
            {
                case ComponentName::Transform:
                    iconID = m_TransformIcon->GetRendererID();
                    break;
                case ComponentName::Camera:
                    iconID = m_CameraIcon->GetRendererID();
                    break;
                case ComponentName::SpriteRenderer:
                    iconID = m_SpriteRendererIcon->GetRendererID();
                    break;
                case ComponentName::None:
                    break;
            }
            
            // 组件 Icon
            ImGui::SameLine();
            ImGui::SetCursorPosY(nodePos.y + padding * 1.5);
            ImGui::PushFont(boldFont);
            ImGui::Image((void*)iconID, ImVec2(lineHeight - padding, lineHeight - padding), ImVec2(0, 1), ImVec2(1, 0));
            ImGui::PopFont();

            // 组件名字文本
            ImGui::SameLine(nodePos.x + lineHeight * 2.6);
            ImGui::SetCursorPosY(nodePos.y);
            ImGui::PushFont(boldFont);
            ImGui::Text(name.c_str());
            ImGui::PopFont();

            ImGui::SameLine(contentRegionAvail.x - lineHeight - padding * 2);      // 同一行：可用区域左移
            ImGui::SetCursorPosY(nodePos.y + padding);
            // 组件设置按钮
            if (ImGui::ImageButton((void*)m_SettingsButtonIcon->GetRendererID(), { lineHeight - padding * 2, lineHeight - padding * 2 }, ImVec2(0, 1), ImVec2(1, 0)))
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
}