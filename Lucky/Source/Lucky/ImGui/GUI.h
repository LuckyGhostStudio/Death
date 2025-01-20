#pragma once

#include <imgui/imgui.h>
#include <glm/glm.hpp>

namespace Lucky
{
    /*
    * TODO option : styles, params
    * 
    * DragFloat
    * DragFloat2
    * DragFloat3
    * SliderFloat
    * SliderAngle
    * 
    * TODO List:
    * TreeNode�����������ļ�����Ҫ�õ�
    * TreeNodeOther��������������ڵ�
    * ����ѡ������������ʾ��ǰ�����ö�����ʲ�
    */

    /// <summary>
    /// �༭�� GUI
    /// </summary>
    class GUI
    {
    public:
        static bool DragFloat(const std::string& label, float* value, float valueSpeed = 0.01f, float minValue = 0.0f, float maxValue = 0.0f, float labelMinWidth = 120.0f, float widgetOffset = 40.0f);
        static bool DragFloat2(const std::string& label, glm::vec2& values, float valueSpeed = 0.01f, float minValue = 0.0f, float maxValue = 0.0f, float labelMinWidth = 120.0f, float widgetOffset = 83.0f);
        static bool DragFloat3(const std::string& label, glm::vec3& values, float valueSpeed = 0.01f, float minValue = 0.0f, float maxValue = 0.0f, float labelMinWidth = 120.0f, float widgetOffset = 106.0f);

        static bool SliderFloat(const std::string& label, float* value, float minValue = 0.0f, float maxValue = 0.0f, float labelMinWidth = 120.0f, float widgetOffset = 40.0f);
        static bool SliderAngle(const std::string& label, float* value, float minValue = 0.0f, float maxValue = 0.0f, float labelMinWidth = 120.0f, float widgetOffset = 40.0f);

        static bool Toggle(const std::string& label, bool* value, float labelMinWidth = 120.0f, float widgetOffset = 40.0f);

        static bool ColorEditor4(const std::string& label, glm::vec4& color, float labelMinWidth = 120.0f, float widgetOffset = 40.0f);

        /// <summary>
        /// ��ѡ�������б�
        /// </summary>
        /// <param name="label">��ǩ��</param>
        /// <param name="options">ѡ���б�</param>
        /// <param name="optionCount">ѡ�����</param>
        /// <param name="selected">ѡ�����±�</param>
        /// <param name="labelMinWidth">��ǩ��С�п�</param>
        /// <param name="widgetOffset">С�����ұ߽�����ƫ����</param>
        static bool DropdownList(const std::string& label, const char** options, uint32_t optionCount, int* selected, float labelMinWidth = 120.0f, float widgetOffset = 40.0f);

        static bool ImageButton(const std::string& label, uint32_t textureID, const glm::vec2& size, float framePadding = 2.0f, float labelMinWidth = 120.0f, float widgetOffset = 40.0f);

        template<typename Func>
        inline static void ObjectField(const std::string& label, uint32_t textureID, const std::string& objectName, Func OnClicked, float labelMinWidth = 120.0f, float widgetOffset = 40.0f);

        static bool SelectableImageButton(uint32_t textureID, const glm::vec2& size, const glm::vec4 colors[4], float framePadding = -1.0f);

        template<typename T, typename Func>
        inline static void Foldout(const std::string& label, uint32_t textureID, Func OnOpened, bool defaultOpen = true);
    };

    template<typename Func>
    inline void GUI::ObjectField(const std::string& label, uint32_t textureID, const std::string& objectName, Func OnClicked, float labelMinWidth, float widgetOffset)
    {
        ImGui::PushID(label.c_str());   // ���ÿؼ� ID

        float lineHeight = ImGui::GetIO().Fonts->Fonts[1]->FontSize;
        float padding = ImGui::GetStyle().FramePadding.y;

        float panelWidth = ImGui::GetWindowContentRegionWidth();    // �����
        // ���� label ��� [labelMinWidth, panelWidth * 0.4f]
        float labelWidth = panelWidth * 0.4f;
        if (labelWidth < labelMinWidth)
        {
            labelWidth = labelMinWidth;
        }

        ImGui::Columns(2, 0, false);            // ����Ϊ ���� id �߽�ȡ����ʾ
        ImGui::SetColumnWidth(0, labelWidth);   // ���� 0 ���п�

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding);
        ImGui::Text(label.c_str()); // �ؼ�����0 ���У�

        ImGui::NextColumn();
        ImGui::PushItemWidth(panelWidth - labelWidth - widgetOffset); // ���� 1 ���п� = ���� - ��ǩ�п� - С�����ұ߽�����ƫ����


        // ��ť��1 ���У�
        float posX = ImGui::GetCursorPosX();
        if (ImGui::Button("##Name", ImVec2(panelWidth - labelWidth - widgetOffset, 0)))
        {
            OnClicked();   // ����ص�����
        }

        // ����ͼ��
        ImGui::SameLine();
        ImGui::SetCursorPos({ posX + padding * 1.5f, ImGui::GetCursorPosY() + padding * 1.6f });
        ImGui::Image((ImTextureID)textureID, ImVec2(lineHeight - padding, lineHeight - padding), ImVec2(0, 1), ImVec2(1, 0));

        // ������ TODO None (ObjectType ģ�����)
        ImGui::SameLine();
        ImGui::SetCursorPos({ ImGui::GetCursorPosX() - padding, ImGui::GetCursorPosY() - padding * 1.6f });
        ImGui::Text(objectName.empty() ? "(None)" : objectName.c_str());

        ImGui::PopItemWidth();
        ImGui::Columns(1);      // ����Ϊһ��

        ImGui::PopID();
    }

    template<typename T, typename Func>
    inline void GUI::Foldout(const std::string& label, uint32_t iconID, Func OnOpened, bool defaultOpen)
    {
        ImGui::PushID(label.c_str());   // ���ÿؼ�ID

        // ������־
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (defaultOpen)
        {
            flags |= ImGuiTreeNodeFlags_DefaultOpen;
        }

        ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();     // ���������С

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 1));   // Separator �� TreeNodeEx ֮��Ĵ�ֱ����Ϊ 1
        ImGui::Separator(); // �ָ���
        ImGui::Separator(); // �ָ���

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);            // �߿�Բ��
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);          // �߿��߿� 0

        ImFont* boldFont = ImGui::GetIO().Fonts->Fonts[0];  // ����

        float lineHeight = boldFont->FontSize;
        float padding = ImGui::GetStyle().FramePadding.y;

        ImVec2 nodePos = ImGui::GetCursorPos();

        static ImVec4 headerColor;

        // ��ϣֵ��Ϊ��� id
        bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, "");

        if (ImGui::IsItemHovered())
        {
            headerColor = ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered];
        }
        else
        {
            headerColor = ImGui::GetStyle().Colors[ImGuiCol_Header];
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
        ImGui::Text(label.c_str());
        ImGui::PopFont();
        
        //ImGui::SameLine(contentRegionAvail.x - lineHeight - padding * 2);      // ͬһ�У�������������
        //ImGui::SetCursorPosY(nodePos.y + padding);
        //// ������ð�ť
        //ImGui::PushStyleColor(ImGuiCol_Button, headerColor);
        //if (ImGui::ImageButton((void*)m_SettingsButtonIcon->GetRendererID(), { lineHeight - padding * 2, lineHeight - padding * 2 }, ImVec2(0, 1), ImVec2(1, 0)))
        //{
        //    ImGui::OpenPopup("Settings");   // �򿪵�����
        //}
        //ImGui::PopStyleColor();

        ImGui::PopStyleVar(4);
        /*
        // ��Ⱦ������
        if (ImGui::BeginPopup("Settings"))
        {
            // TODO Settings Funtion

            ImGui::EndPopup();
        }
        */

        ImGui::SetCursorPosY(ImGui::GetCursorPos().y - padding);
        ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
        ImGui::Separator();
        ImGui::PopStyleColor();

        if (opened)
        {
            OnOpened();

            ImGui::TreePop();
        }

        ImGui::PopID();
    }
}