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
    /// ������壺��ʾѡ��������ʲ�����Ϣ
    /// </summary>
    class InspectorPanel : public EditorPanel
    {
    private:
        Ref<Scene> m_Scene;

        Ref<Texture2D> m_SettingsButtonIcon;    // ������ð�ť Icon
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
        /// ���������� UI
        /// </summary>
        /// <param name="object"></param>
        void DrawAddComponents(Object object);

        /// <summary>
        /// ���������������� UI
        /// </summary>
        /// <param name="object">����</param>
        void DrawComponents(Object object);
    public:
        InspectorPanel();
        InspectorPanel(const Ref<Scene>& scene);

        virtual void SetSceneContext(const Ref<Scene>& scene) override;
        Ref<Scene> GetSceneContext() const { return m_Scene; }

        virtual void OnImGuiRender(bool& isOpen) override;
    private:
        /// <summary>
        /// �������
        /// </summary>
        /// <typeparam name="TComponent">�������</typeparam>
        /// <typeparam name="UIFunction">������ܺ�������</typeparam>
        /// <param name="name">�����</param>
        /// <param name="object">ʵ��</param>
        /// <param name="OnOpened">�����ʱ����</param>
        template<typename TComponent, typename UIFunction>
        void DrawComponent(const std::string& name, Object object, UIFunction OnOpened);

        /// <summary>
        /// ������������ť
        /// </summary>
        /// <typeparam name="TComponent">�������</typeparam>
        /// <param name="name"></param>
        template<typename TComponent>
        void DrawAddComponentItemButton(const std::string& name);
    };

    template<typename TComponent, typename UIFunction>
    inline void InspectorPanel::DrawComponent(const std::string& name, Object object, UIFunction OnOpened)
    {
        // TComponent �������
        if (object.HasComponent<TComponent>())
        {
            // ���ڵ��־����|���|���쵽�ұ�|�����ص�|��ܱ߿�
            const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth;
            
            auto& component = object.GetComponent<TComponent>();            // ������

            ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();     // ���������С

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 1));   // Separator �� TreeNodeEx ֮��Ĵ�ֱ����Ϊ 1
            ImGui::Separator(); // �ָ���
            ImGui::Separator(); // �ָ���

            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);            // �߿�Բ��
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);          // �߿��߿� 0

            ImFont* boldFont = ImGui::GetIO().Fonts->Fonts[0];  // ����

            float lineHeight = GImGui->Font->FontSize;
            float padding = GImGui->Style.FramePadding.y;

            ImVec2 nodePos = ImGui::GetCursorPos();

            static ImVec4 headerColor;

            // �����㣺�����Ĺ�ϣֵ��Ϊ��� id
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
            
            // ��� Icon
            ImGui::SameLine();
            ImGui::SetCursorPosY(nodePos.y + padding * 1.5);
            ImGui::PushFont(boldFont);
            ImGui::Image((void*)iconID, ImVec2(lineHeight - padding, lineHeight - padding), ImVec2(0, 1), ImVec2(1, 0));
            ImGui::PopFont();

            // ��������ı�
            ImGui::SameLine(nodePos.x + lineHeight * 2.6);
            ImGui::SetCursorPosY(nodePos.y);
            ImGui::PushFont(boldFont);
            ImGui::Text(name.c_str());
            ImGui::PopFont();

            ImGui::SameLine(contentRegionAvail.x - lineHeight - padding * 2);      // ͬһ�У�������������
            ImGui::SetCursorPosY(nodePos.y + padding);
            // ������ð�ť
            ImGui::PushStyleColor(ImGuiCol_Button, headerColor );
            if (ImGui::ImageButton((void*)m_SettingsButtonIcon->GetRendererID(), { lineHeight - padding * 2, lineHeight - padding * 2 }, ImVec2(0, 1), ImVec2(1, 0)))
            {
                ImGui::OpenPopup("ComponentSettings");  // �򿪵�����
            }
            ImGui::PopStyleColor();

            ImGui::PopStyleVar(4);

            // �Ƴ����
            bool componentRemoved = false;
            // ��Ⱦ������
            if (ImGui::BeginPopup("ComponentSettings"))
            {
                // �Ƴ�����˵���
                if (ImGui::MenuItem("Remove Component"))
                {
                    componentRemoved = true;    // ������Ϊ�Ƴ�
                }

                ImGui::EndPopup();
            }

            ImGui::SetCursorPosY(ImGui::GetCursorPos().y - padding);
            if (opened)
            {
                ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
                ImGui::Separator();
                ImGui::PopStyleColor();

                OnOpened(component);    // ����������ܺ��������Ƹ������ͬ�Ĳ���

                ImGui::TreePop();       // չ�����
            }

            if (componentRemoved)
            {
                object.RemoveComponent<TComponent>();    // �Ƴ� TComponent ���
            }
        }
    }

    template<typename TComponent>
    inline void InspectorPanel::DrawAddComponentItemButton(const std::string& name)
    {
        // TODO �жϸ�����Ƿ������Ӷ��
        if (SelectionManager::GetSelection().HasComponent<TComponent>())
        {
            // TODO �������� MessageBox
            return;
        }

        // ��� TComponent ���
        if (ImGui::MenuItem(name.c_str()))
        {
            SelectionManager::GetSelection().AddComponent<TComponent>();

            ImGui::CloseCurrentPopup();
        }
    }
}