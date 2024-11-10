#include "lcpch.h"
#include "GUI.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Lucky
{
    void GUI::DragFloat3(const std::string& label, glm::vec3& values, float valueSpeed, float minValue, float maxValue, float labelMinWidth, float widgetOffset)
    {
        ImGui::PushID(label.c_str());   // ���ÿؼ� ID

        ImFont* boldFont = ImGui::GetIO().Fonts->Fonts[0];  // ����

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
        ImGui::PushMultiItemsWidths(3, panelWidth - labelWidth - widgetOffset); // ���� 1 ���п� = ���� - ��ǩ�п� - С�����ұ߽�����ƫ����

        ImGui::PushFont(boldFont);
        // �϶�����1 ���У�
        ImGui::Text("X");

        ImGui::PopFont();

        ImGui::SameLine();

        ImGui::DragFloat("##X", &values.x, valueSpeed, minValue, maxValue);

        ImGui::PopItemWidth();

        ImGui::SameLine();

        ImGui::PushFont(boldFont);
        // �϶�����2 ���У�
        ImGui::Text("Y");

        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, valueSpeed, minValue, maxValue);

        ImGui::PopItemWidth();

        ImGui::SameLine();

        ImGui::PushFont(boldFont);
        // �϶�����3 ���У�
        ImGui::Text("Z");

        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &values.z, valueSpeed, minValue, maxValue);

        ImGui::PopItemWidth();

        ImGui::Columns(1);  // ����Ϊһ��

        ImGui::PopID();
    }

    void GUI::DragFloatN(const std::string& label, float* value, float valueSpeed, ValueType valueType, float minValue, float maxValue, float labelMinWidth, float widgetOffset)
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

        // �϶�����1 ���У�
        switch (valueType)
        {
            case ValueType::Float:
                ImGui::DragFloat("##Float", value, valueSpeed, minValue, maxValue);
                break;
            case ValueType::Float2:
                ImGui::DragFloat2("##Float2", value, valueSpeed, minValue, maxValue);
                break;
            case ValueType::Float3:
                ImGui::DragFloat3("##Float3", value, valueSpeed, minValue, maxValue);
        }

        ImGui::PopItemWidth();
        ImGui::Columns(1);  // ����Ϊһ��

        ImGui::PopID();
    }

    void GUI::DragSlider(const std::string& label, float* value, float minValue, float maxValue, ValueType valueType, float labelMinWidth, float widgetOffset)
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

        ImGui::Checkbox("##None", value);   // ��ѡ��1 ���У�
        
        ImGui::PopItemWidth();
        ImGui::Columns(1);      // ����Ϊһ��

        ImGui::PopID();
    }
}
