#pragma once

#include <imgui/imgui.h>
#include <glm/glm.hpp>

namespace Lucky
{
    /*
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
        /// <summary>
        /// ֵ����
        /// </summary>
        enum class ValueType
        {
            None = 0,

            Float,
            Float2,
            Float3,

            Angle,
        };
    public:
        /// <summary>
        /// Drag Float3
        /// </summary>
        /// <param name="label">��ǩ��</param>
        /// <param name="value">ֵ</param>
        /// <param name="valueSpeed">�϶�����</param>
        /// <param name="minValue">��Сֵ</param>
        /// <param name="maxValue">���ֵ</param>
        /// <param name="labelMinWidth">��ǩ��С�п�</param>
        /// <param name="widgetOffset">С�����ұ߽�����ƫ����</param>
        static void DragFloat3(const std::string& label, glm::vec3& values, float valueSpeed = 0.01f, float minValue = 0.0f, float maxValue = 0.0f, float labelMinWidth = 120.0f, float widgetOffset = 105.0f);
   
        /// <summary>
        /// Drag FloatN
        /// </summary>
        /// <param name="label">��ǩ��</param>
        /// <param name="value">ֵ</param>
        /// <param name="valueSpeed">�϶�����</param>
        /// <param name="valueType">ֵ����</param>
        /// <param name="minValue">��Сֵ</param>
        /// <param name="maxValue">���ֵ</param>
        /// <param name="labelMinWidth">��ǩ��С�п�</param>
        /// <param name="widgetOffset">С�����ұ߽�����ƫ����</param>
        static void DragFloatN(const std::string& label, float* value, float valueSpeed = 0.01f, ValueType valueType = ValueType::Float, float minValue = 0.0f, float maxValue = 0.0f, float labelMinWidth = 120.0f, float widgetOffset = 40.0f);
    
        /// <summary>
        /// Drag Slider
        /// </summary>
        /// <param name="label">��ǩ��</param>
        /// <param name="value">ֵ</param>
        /// <param name="minValue">��Сֵ</param>
        /// <param name="maxValue">���ֵ</param>
        /// <param name="valueType">ֵ����</param>
        /// <param name="labelMinWidth">��ǩ��С�п�</param>
        /// <param name="widgetOffset">С�����ұ߽�����ƫ����</param>
        static void DragSlider(const std::string& label, float* value, float minValue, float maxValue, ValueType valueType = ValueType::Float, float labelMinWidth = 120.0f, float widgetOffset = 40.0f);
    
        /// <summary>
        /// ��ѡ��
        /// </summary>
        /// <param name="label">��ǩ��</param>
        /// <param name="value">ֵ</param>
        /// <param name="labelMinWidth">��ǩ��С�п�</param>
        /// <param name="widgetOffset">С�����ұ߽�����ƫ����</param>
        static void CheckBox(const std::string& label, bool* value, float labelMinWidth = 120.0f, float widgetOffset = 40.0f);

        /// <summary>
        /// ��ɫ�༭��
        /// </summary>
        /// <param name="label">��ǩ��</param>
        /// <param name="color">��ɫֵ</param>
        /// <param name="labelMinWidth"></param>
        /// <param name="widgetOffset"></param>
        static void ColorEditor4(const std::string& label, glm::vec4& color, float labelMinWidth = 120.0f, float widgetOffset = 40.0f);

        /// <summary>
        /// ��ѡ�������б�
        /// </summary>
        /// <typeparam name="Func">ĳ��ѡ��ʱ�ص�����</typeparam>
        /// <param name="label">��ǩ��</param>
        /// <param name="currentValueStr">��ǰֵ</param>
        /// <param name="valueStrs">���п�ѡֵ�б�</param>
        /// <param name="valueStrCount">ֵ����</param>
        /// <param name="OnSelected">ĳ��ѡ��ʱ�ص�������������int index ���ţ�const char* value ��ֵ��</param>
        /// <param name="labelMinWidth">��ǩ��С�п�</param>
        /// <param name="widgetOffset">С�����ұ߽�����ƫ����</param>
        template<typename Func>
        inline static void DropdownList(const std::string& label, const char* currentValueStr, const char* valueStrs[], uint32_t valueStrCount, Func OnSelected, float labelMinWidth = 120.0f, float widgetOffset = 40.0f);

        template<typename Func>
        inline static void ObjectSelector(const std::string& label, uint32_t textureID, const glm::vec2& size, Func OnSelected, float framePadding = 2.0f, float labelMinWidth = 120.0f, float widgetOffset = 40.0f);

        template<typename Func>
        inline static void SelectableImageButton(uint32_t textureID, const glm::vec2& size, const glm::vec4 colors[4], Func OnClicked, float framePadding = -1.0f);
    };

    template<typename Func>
    inline void GUI::DropdownList(const std::string& label, const char* currentValueStr, const char* valueStrs[], uint32_t valueStrCount, Func OnSelected, float labelMinWidth, float widgetOffset)
    {
        ImGui::PushID(label.c_str());   // ���ÿؼ� ID

        float panelWidth = ImGui::GetWindowContentRegionWidth();    // �����
        // ���� label ��� [labelMinWidth, panelWidth * 0.4f]
        float labelWidth = panelWidth * 0.4f;
        if (labelWidth < labelMinWidth)
        {
            labelWidth = labelMinWidth;
        }

        ImGui::Columns(2, 0, false);            // ����Ϊ ���� id �߽�ȡ����ʾ
        ImGui::SetColumnWidth(0, labelWidth);   // ���� 0 ���п�

        ImGui::Text(label.c_str()); // �ؼ�����0 ���У�

        ImGui::NextColumn();
        ImGui::PushItemWidth(panelWidth - labelWidth - widgetOffset); // ���� 1 ���п� = ���� - ��ǩ�п� - С�����ұ߽�����ƫ����

        // ������1 ���У�
        if (ImGui::BeginCombo("##None", currentValueStr))
        {
            // ����ѡ����
            for (int i = 0; i < valueStrCount; i++)
            {
                bool isSelected = currentValueStr == valueStrs[i]; // ��ѡ�У���ǰֵ==��i��ֵ
                // ��ѡ�������ı�ʱ����i��Ϊѡ����
                if (ImGui::Selectable(valueStrs[i], isSelected))
                {
                    currentValueStr = valueStrs[i]; // ���õ�ǰֵ
                    OnSelected(i, currentValueStr); // i ��ѡ���¼�����
                }

                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();   // ����Ĭ��ѡ����
                }
            }
            ImGui::EndCombo();
        }

        ImGui::PopItemWidth();
        ImGui::Columns(1);      // ����Ϊһ��

        ImGui::PopID();
    }

    template<typename Func>
    inline void GUI::ObjectSelector(const std::string& label, uint32_t textureID, const glm::vec2& size, Func OnSelected, float framePadding, float labelMinWidth, float widgetOffset)
    {
        ImGui::PushID(label.c_str());   // ���ÿؼ� ID

        float panelWidth = ImGui::GetWindowContentRegionWidth();    // �����
        // ���� label ��� [labelMinWidth, panelWidth * 0.4f]
        float labelWidth = panelWidth * 0.4f;
        if (labelWidth < labelMinWidth)
        {
            labelWidth = labelMinWidth;
        }

        ImGui::Columns(2, 0, false);            // ����Ϊ ���� id �߽�ȡ����ʾ
        ImGui::SetColumnWidth(0, labelWidth);   // ���� 0 ���п�

        ImGui::Text(label.c_str()); // �ؼ�����0 ���У�

        ImGui::NextColumn();
        ImGui::PushItemWidth(panelWidth - labelWidth - widgetOffset); // ���� 1 ���п� = ���� - ��ǩ�п� - С�����ұ߽�����ƫ����

        // TODO ���������Դ��λ OnLocated
        // Icon
        // Text

        // TODO ���������Դ�����ѡ��
        // ��ť��1 ���У�
        if (ImGui::ImageButton((ImTextureID)textureID, ImVec2(size.x, size.y), ImVec2(0, 1), ImVec2(1, 0), framePadding))
        {
            OnSelected();   // ������Դ/����ѡ��ص�����
        }

        ImGui::PopItemWidth();
        ImGui::Columns(1);      // ����Ϊһ��

        ImGui::PopID();
    }

    template<typename Func>
    inline void GUI::SelectableImageButton(uint32_t textureID, const glm::vec2& size, const glm::vec4 colors[4], Func OnClicked, float framePadding)
    {
        ImGui::PushID(textureID);   // ���ÿؼ� ID

        static bool isSelected = false;

        static ImVec4 buttonColor = { colors[0].r, colors[0].g, colors[0].b,colors[0].a, };         // ԭɫ
        static ImVec4 buttonHoveredColor = { colors[1].r, colors[1].g, colors[1].b,colors[1].a, };  // ������ɫ
        static ImVec4 buttonActiveColor = { colors[2].r, colors[2].g, colors[2].b,colors[2].a, };   // ������ɫ

        if (isSelected)
        {
            buttonColor = { colors[3].r, colors[3].g, colors[3].b,colors[3].a, };
            buttonHoveredColor = { colors[3].r, colors[3].g, colors[3].b,colors[3].a, };
            buttonActiveColor = { colors[3].r, colors[3].g, colors[3].b,colors[3].a, };
        }
        else
        {
            buttonColor = { colors[0].r, colors[0].g, colors[0].b,colors[0].a, };
            buttonHoveredColor = { colors[1].r, colors[1].g, colors[1].b,colors[1].a, };
            buttonActiveColor = { colors[2].r, colors[2].g, colors[2].b,colors[2].a, };
        }

        ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonHoveredColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, buttonActiveColor);
        if (ImGui::ImageButton((ImTextureID)textureID, ImVec2(size.x, size.y), ImVec2(0, 1), ImVec2(1, 0), framePadding))
        {
            isSelected = !isSelected;

            OnClicked();
        }
        ImGui::PopStyleColor(3);

        ImGui::PopID();
    }
}