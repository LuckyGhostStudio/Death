#pragma once

#include "EditorPanel.h"

#include "Lucky/Scene/Object.h"
#include "Lucky/Scene/SelectionManager.h"

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
        Ref<Scene> m_Scene;

        Ref<Texture2D> m_SettingsButtonIcon;    // 组件设置按钮 Icon
        Ref<Texture2D> m_ComponentIcon;
        Ref<Texture2D> m_TransformIcon;
        Ref<Texture2D> m_CameraIcon;
        Ref<Texture2D> m_SpriteRendererIcon;
        Ref<Texture2D> m_Rigidbody2DIcon;
        Ref<Texture2D> m_BoxCollider2DIcon;
        Ref<Texture2D> m_CircleCollider2DIcon;
        Ref<Texture2D> m_ScriptIcon;
    private:
        /// <summary>
        /// 绘制添加组件 UI
        /// </summary>
        /// <param name="object"></param>
        void DrawAddComponents(Object object);

        /// <summary>
        /// 绘制物体的所有组件 UI
        /// </summary>
        /// <param name="object">物体</param>
        void DrawComponents(Object object);
    public:
        InspectorPanel();
        InspectorPanel(const Ref<Scene>& scene);

        virtual void SetSceneContext(const Ref<Scene>& scene) override;
        Ref<Scene> GetSceneContext() const { return m_Scene; }

        virtual void OnImGuiRender(bool& isOpen) override;
    private:
        /// <summary>
        /// 绘制组件
        /// </summary>
        /// <typeparam name="TComponent">组件类型</typeparam>
        /// <typeparam name="UIFunction">组件功能函数类型</typeparam>
        /// <param name="name">组件名</param>
        /// <param name="object">实体</param>
        /// <param name="OnOpened">组件打开时调用</param>
        template<typename TComponent, typename UIFunction>
        void DrawComponent(const std::string& name, Object object, UIFunction OnOpened);

        /// <summary>
        /// 绘制添加组件按钮
        /// </summary>
        /// <typeparam name="TComponent">组件类型</typeparam>
        /// <param name="name"></param>
        template<typename TComponent>
        void DrawAddComponentItemButton(const std::string& name);
    };

    template<typename TComponent, typename UIFunction>
    inline void InspectorPanel::DrawComponent(const std::string& name, Object object, UIFunction OnOpened)
    {
        // TComponent 组件存在
        if (object.HasComponent<TComponent>())
        {
            // 树节点标志：打开|框架|延伸到右边|允许重叠|框架边框
            const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth;
            
            auto& component = object.GetComponent<TComponent>();            // 获得组件

            ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();     // 可用区域大小

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 1));   // Separator 和 TreeNodeEx 之间的垂直距离为 1
            ImGui::Separator(); // 分隔符
            ImGui::Separator(); // 分隔符

            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);            // 边框圆度
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);          // 边框线宽 0

            ImFont* boldFont = ImGui::GetIO().Fonts->Fonts[0];  // 粗体

            float lineHeight = GImGui->Font->FontSize;
            float padding = GImGui->Style.FramePadding.y;

            ImVec2 nodePos = ImGui::GetCursorPos();

            static ImVec4 headerColor;

            // 组件结点：组件类的哈希值作为结点 id
            bool opened = ImGui::TreeNodeEx((void*)typeid(TComponent).hash_code(), flags, "");

            if (ImGui::IsItemHovered())
            {
                headerColor = ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered];
            }
            else
            {
                headerColor = ImGui::GetStyle().Colors[ImGuiCol_Header];
            }

            uint32_t iconID = 0;
            switch (component.Type)
            {
                case ComponentType::Transform:          iconID = m_TransformIcon->GetRendererID();          break;
                case ComponentType::Camera:             iconID = m_CameraIcon->GetRendererID();             break;
                case ComponentType::SpriteRenderer:     iconID = m_SpriteRendererIcon->GetRendererID();     break;
                case ComponentType::Rigidbody2D:        iconID = m_Rigidbody2DIcon->GetRendererID();        break;
                case ComponentType::BoxCollider2D:      iconID = m_BoxCollider2DIcon->GetRendererID();      break;
                case ComponentType::CircleCollider2D:   iconID = m_CircleCollider2DIcon->GetRendererID();   break;
                case ComponentType::Script:             iconID = m_ScriptIcon->GetRendererID();             break;
                case ComponentType::None: break;
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
            ImGui::PushStyleColor(ImGuiCol_Button, headerColor );
            if (ImGui::ImageButton((void*)m_SettingsButtonIcon->GetRendererID(), { lineHeight - padding * 2, lineHeight - padding * 2 }, ImVec2(0, 1), ImVec2(1, 0)))
            {
                ImGui::OpenPopup("ComponentSettings");  // 打开弹出框
            }
            ImGui::PopStyleColor();

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

            ImGui::SetCursorPosY(ImGui::GetCursorPos().y - padding);
            if (opened)
            {
                ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
                ImGui::Separator();
                ImGui::PopStyleColor();

                OnOpened(component);    // 调用组件功能函数：绘制该组件不同的部分

                ImGui::TreePop();       // 展开结点
            }

            if (componentRemoved)
            {
                object.RemoveComponent<TComponent>();    // 移除 TComponent 组件
            }
        }
    }

    template<typename TComponent>
    inline void InspectorPanel::DrawAddComponentItemButton(const std::string& name)
    {
        // TODO 判断该组件是否可以添加多个
        if (SelectionManager::GetSelection().HasComponent<TComponent>())
        {
            // TODO 弹出错误 MessageBox
            return;
        }

        // 添加 TComponent 组件
        if (ImGui::MenuItem(name.c_str()))
        {
            SelectionManager::GetSelection().AddComponent<TComponent>();

            ImGui::CloseCurrentPopup();
        }
    }
}