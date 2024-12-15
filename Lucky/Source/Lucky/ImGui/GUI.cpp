#include "lcpch.h"
#include "GUI.h"

#include <imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

namespace Lucky
{
    void GUI::DragFloat3(const std::string& label, glm::vec3& values, float valueSpeed, float minValue, float maxValue, float labelMinWidth, float widgetOffset)
    {
        ImGui::PushID(label.c_str());   // ���ÿؼ� ID

        ImFont* boldFont = ImGui::GetIO().Fonts->Fonts[0];  // ����
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
        ImGui::PushMultiItemsWidths(3, panelWidth - labelWidth - widgetOffset); // ���� 1 ���п� = ���� - ��ǩ�п� - С�����ұ߽�����ƫ����

        // �϶�����1 ���У�
        ImGui::PushFont(boldFont);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding);
        ImGui::Text("X");
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - padding);
        ImGui::DragFloat("##X", &values.x, valueSpeed, minValue, maxValue);

        ImGui::PopItemWidth();

        ImGui::SameLine();

        // �϶�����2 ���У�
        ImGui::PushFont(boldFont);
        ImGui::Text("Y");
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, valueSpeed, minValue, maxValue);

        ImGui::PopItemWidth();

        ImGui::SameLine();

        // �϶�����3 ���У�
        ImGui::PushFont(boldFont);
        ImGui::Text("Z");
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &values.z, valueSpeed, minValue, maxValue);

        ImGui::PopItemWidth();

        ImGui::Columns(1);  // ����Ϊһ��

        ImGui::PopID();
    }

    bool GUI::DragFloatN(const std::string& label, float* value, float valueSpeed, ValueType valueType, float minValue, float maxValue, float labelMinWidth, float widgetOffset)
    {
        ImGui::PushID(label.c_str());   // ���ÿؼ� ID

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

        bool changed = false;

        // �϶�����1 ���У�
        switch (valueType)
        {
            case ValueType::Float:
                changed = ImGui::DragFloat("##Float", value, valueSpeed, minValue, maxValue);
                break;
            case ValueType::Float2:
                changed = ImGui::DragFloat2("##Float2", value, valueSpeed, minValue, maxValue);
                break;
            case ValueType::Float3:
                changed = ImGui::DragFloat3("##Float3", value, valueSpeed, minValue, maxValue);
        }

        ImGui::PopItemWidth();
        ImGui::Columns(1);  // ����Ϊһ��

        ImGui::PopID();

        return changed;
    }

    void GUI::DragSlider(const std::string& label, float* value, float minValue, float maxValue, ValueType valueType, float labelMinWidth, float widgetOffset)
    {
        ImGui::PushID(label.c_str());   // ���ÿؼ� ID

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

        // ��������1 ���У�
        switch (valueType)
        {
            case ValueType::Float:
                ImGui::SliderFloat("##Float", value, minValue, maxValue);
                break;
            case ValueType::Angle:
                ImGui::SliderAngle("##Angle", value, minValue, maxValue);   // ����
                break;
        }

        ImGui::PopItemWidth();
        ImGui::Columns(1);  // ����Ϊһ��

        ImGui::PopID();
    }

    void GUI::CheckBox(const std::string& label, bool* value, float labelMinWidth, float widgetOffset)
    {
        ImGui::PushID(label.c_str());   // ���ÿؼ�ID

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

        ImGui::Checkbox("##None", value);   // ��ѡ��1 ���У�
        
        ImGui::PopItemWidth();
        ImGui::Columns(1);      // ����Ϊһ��

        ImGui::PopID();
    }

    void GUI::ColorEditor4(const std::string& label, glm::vec4& color, float labelMinWidth, float widgetOffset)
    {
        ImGui::PushID(label.c_str());   // ���ÿؼ�ID
#if 1   // TODO �ƶ���ͨ��
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
#endif
        ImGui::ColorEdit4("##None", glm::value_ptr(color)); // ��ɫ�༭����1���У�

        ImGui::PopItemWidth();
        ImGui::Columns(1);      // ����Ϊһ��

        ImGui::PopID();
    }
}
