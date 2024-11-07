#include "lcpch.h"
#include "GUI.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Lucky
{
    void GUI::DragVector3(const std::string& label, glm::vec3& values, float resetValue, float columnWidth)
    {
        ImGuiIO& io = ImGui::GetIO();
        auto boldFont = io.Fonts->Fonts[0]; // ���壺0 ������

        ImGui::PushID(label.c_str());       // ���ÿؼ� ID

        float panelWidth = ImGui::GetWindowContentRegionWidth();    // �����

        // ��ǩ��
        ImGui::Columns(2, 0, false);            // ����Ϊ ���� id �߽� 0
        ImGui::SetColumnWidth(0, columnWidth);  // ���� 0 ���п�
        ImGui::Text(label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, panelWidth - columnWidth - 105); // ���� 3 ���п�
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 5, 5 }); // Var Item �ո�

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;    // �и� = �����С + �߿�.y * 2
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };                              // ��ť��С

        // X ���� UI
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 5 });                 // Var Item �ո�
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));        // ��ť��ɫ
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.3f, 0.35f, 1.0f)); // �����ͣ�ڰ�ťʱ����ɫ
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.1f, 0.15f, 1.0f));  // ��ť������ɫ
        
        ImGui::PushFont(boldFont);  // ���ô���

        // X ��ť����
        if (ImGui::Button("X", buttonSize))
        {
            values.x = resetValue;  // ���� x ����
        }

        ImGui::PopFont();

        ImGui::PopStyleColor(3);
        ImGui::SameLine();          // ��ͬһ��
        ImGui::PopStyleVar();

        ImGui::DragFloat("##X", &values.x, 0.01f);  // X ������ ## ����ʾ��ǩ �϶����� 0.01
        ImGui::PopItemWidth();                      // �Ƴ���һ���п�
        ImGui::SameLine();

        // Y ���� UI
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 5 });                 // Var Item �ո�
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));         // ��ť��ɫ
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.9f, 0.4f, 1.0f));  // �����ͣ�ڰ�ťʱ����ɫ
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));   // ��ť������ɫ

        ImGui::PushFont(boldFont);  // ���ô���

        // Y ��ť����
        if (ImGui::Button("Y", buttonSize))
        {
            values.y = resetValue;  // ���� Y ����
        }

        ImGui::PopFont();

        ImGui::PopStyleColor(3);
        ImGui::SameLine();          // ��ͬһ��
        ImGui::PopStyleVar();

        ImGui::DragFloat("##Y", &values.y, 0.01f);  // Y ������ ## ����ʾ��ǩ �϶����� 0.01
        ImGui::PopItemWidth();                      // �Ƴ���һ���п�
        ImGui::SameLine();

        // Z ���� UI
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 5 });                 // Var Item �ո�
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.25f, 0.8f, 1.0f));        // ��ť��ɫ
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.4f, 0.8f, 1.0f));  // �����ͣ�ڰ�ťʱ����ɫ
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35f, 0.25f, 0.9f, 1.0f)); // ��ť������ɫ

        ImGui::PushFont(boldFont);  // ���ô���

        // Z ��ť����
        if (ImGui::Button("Z", buttonSize))
        {
            values.z = resetValue;  // ���� Z ����
        }

        ImGui::PopFont();

        ImGui::PopStyleColor(3);
        ImGui::SameLine();          // ��ͬһ��
        ImGui::PopStyleVar();

        ImGui::DragFloat("##Z", &values.z, 0.01f);  // Z ������ ## ����ʾ��ǩ �϶����� 0.01
        ImGui::PopItemWidth();                      // �Ƴ���һ���п�

        ImGui::PopStyleVar();

        ImGui::Columns(1);  // ����Ϊ 1 ��

        ImGui::PopID();     // �����ؼ� ID
    }

    void GUI::DragFloatN(const std::string& label, float* value, float valueSpeed, ValueType valueType, float minValue, float maxValue, float labelColumnWidth, float widgetOffset)
    {
        ImGui::PushID(label.c_str());   // ���ÿؼ� ID

        float panelWidth = ImGui::GetWindowContentRegionWidth();    // �����

        ImGui::Columns(2, 0, false);                // ����Ϊ ���� id �߽�ȡ����ʾ
        ImGui::SetColumnWidth(0, labelColumnWidth); // ���� 0 ���п�

        ImGui::Text(label.c_str());     // �ؼ�����0 ���У�

        ImGui::NextColumn();

        ImGui::PushItemWidth(panelWidth - labelColumnWidth - widgetOffset); // ���� 1 ���п� = ���� - ��ǩ�п� - С�����ұ߽�����ƫ����

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

    void GUI::DragSlider(const std::string& label, float* value, float minValue, float maxValue, ValueType valueType, float labelColumnWidth, float widgetOffset)
    {
        ImGui::PushID(label.c_str());   // ���ÿؼ� ID

        float panelWidth = ImGui::GetWindowContentRegionWidth();    // �����

        ImGui::Columns(2, 0, false);                // ����Ϊ ���� id �߽�ȡ����ʾ
        ImGui::SetColumnWidth(0, labelColumnWidth); // ���� 0 ���п�

        ImGui::Text(label.c_str());     // �ؼ�����0 ���У�

        ImGui::NextColumn();

        ImGui::PushItemWidth(panelWidth - labelColumnWidth - widgetOffset); // ���� 1 ���п� = ���� - ��ǩ�п� - С�����ұ߽�����ƫ����

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

    void GUI::CheckBox(const std::string& label, bool* value, float labelColumnWidth, float widgetOffset)
    {
        ImGui::PushID(label.c_str());   // ���ÿؼ�ID

        float panelWidth = ImGui::GetWindowContentRegionWidth();    // �����

        ImGui::Columns(2, 0, false);                // ����Ϊ ���� id �߽�ȡ����ʾ
        ImGui::SetColumnWidth(0, labelColumnWidth); // ���� 0 ���п�

        ImGui::Text(label.c_str());     //�ؼ�����0 ���У�

        ImGui::NextColumn();

        ImGui::PushItemWidth(panelWidth - labelColumnWidth - widgetOffset); // ���� 1 ���п� = ���� - ��ǩ�п� - С�����ұ߽�����ƫ����
        ImGui::Checkbox("##None", value);   // ��ѡ��1 ���У�
        ImGui::PopItemWidth();

        ImGui::Columns(1);      // ����Ϊһ��

        ImGui::PopID();
    }
}
