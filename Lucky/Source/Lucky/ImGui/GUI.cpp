#include "lcpch.h"
#include "GUI.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Lucky
{
    void GUI::DragVector3(const std::string& label, glm::vec3& values, float resetValue, float columnWidth)
    {
        ImGuiIO& io = ImGui::GetIO();
        auto boldFont = io.Fonts->Fonts[0]; // 粗体：0 号字体

        ImGui::PushID(label.c_str());       // 设置控件 ID

        float panelWidth = ImGui::GetWindowContentRegionWidth();    // 面板宽度

        // 标签列
        ImGui::Columns(2, 0, false);            // 设置为 两列 id 边界 0
        ImGui::SetColumnWidth(0, columnWidth);  // 设置 0 号列宽
        ImGui::Text(label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, panelWidth - columnWidth - 105); // 设置 3 个列宽
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 5, 5 }); // Var Item 空格

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;    // 行高 = 字体大小 + 边框.y * 2
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };                              // 按钮大小

        // X 分量 UI
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 5 });                 // Var Item 空格
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));        // 按钮颜色
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.3f, 0.35f, 1.0f)); // 鼠标悬停在按钮时的颜色
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.1f, 0.15f, 1.0f));  // 按钮按下颜色
        
        ImGui::PushFont(boldFont);  // 设置粗体

        // X 按钮按下
        if (ImGui::Button("X", buttonSize))
        {
            values.x = resetValue;  // 重置 x 分量
        }

        ImGui::PopFont();

        ImGui::PopStyleColor(3);
        ImGui::SameLine();          // 在同一行
        ImGui::PopStyleVar();

        ImGui::DragFloat("##X", &values.x, 0.01f);  // X 分量列 ## 不显示标签 拖动精度 0.01
        ImGui::PopItemWidth();                      // 推出第一个列宽
        ImGui::SameLine();

        // Y 分量 UI
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 5 });                 // Var Item 空格
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));         // 按钮颜色
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.9f, 0.4f, 1.0f));  // 鼠标悬停在按钮时的颜色
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));   // 按钮按下颜色

        ImGui::PushFont(boldFont);  // 设置粗体

        // Y 按钮按下
        if (ImGui::Button("Y", buttonSize))
        {
            values.y = resetValue;  // 重置 Y 分量
        }

        ImGui::PopFont();

        ImGui::PopStyleColor(3);
        ImGui::SameLine();          // 在同一行
        ImGui::PopStyleVar();

        ImGui::DragFloat("##Y", &values.y, 0.01f);  // Y 分量列 ## 不显示标签 拖动精度 0.01
        ImGui::PopItemWidth();                      // 推出第一个列宽
        ImGui::SameLine();

        // Z 分量 UI
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 5 });                 // Var Item 空格
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.25f, 0.8f, 1.0f));        // 按钮颜色
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.4f, 0.8f, 1.0f));  // 鼠标悬停在按钮时的颜色
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35f, 0.25f, 0.9f, 1.0f)); // 按钮按下颜色

        ImGui::PushFont(boldFont);  // 设置粗体

        // Z 按钮按下
        if (ImGui::Button("Z", buttonSize))
        {
            values.z = resetValue;  // 重置 Z 分量
        }

        ImGui::PopFont();

        ImGui::PopStyleColor(3);
        ImGui::SameLine();          // 在同一行
        ImGui::PopStyleVar();

        ImGui::DragFloat("##Z", &values.z, 0.01f);  // Z 分量列 ## 不显示标签 拖动精度 0.01
        ImGui::PopItemWidth();                      // 推出第一个列宽

        ImGui::PopStyleVar();

        ImGui::Columns(1);  // 设置为 1 列

        ImGui::PopID();     // 弹出控件 ID
    }

    void GUI::DragFloatN(const std::string& label, float* value, float valueSpeed, ValueType valueType, float minValue, float maxValue, float labelColumnWidth, float widgetOffset)
    {
        ImGui::PushID(label.c_str());   // 设置控件 ID

        float panelWidth = ImGui::GetWindowContentRegionWidth();    // 面板宽度

        ImGui::Columns(2, 0, false);                // 设置为 两列 id 边界取消显示
        ImGui::SetColumnWidth(0, labelColumnWidth); // 设置 0 号列宽

        ImGui::Text(label.c_str());     // 控件名（0 号列）

        ImGui::NextColumn();

        ImGui::PushItemWidth(panelWidth - labelColumnWidth - widgetOffset); // 设置 1 号列宽 = 面板宽 - 标签列宽 - 小部件右边界向左偏移量

        // 拖动条（1 号列）
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

        ImGui::Columns(1);  // 设置为一列

        ImGui::PopID();
    }

    void GUI::DragSlider(const std::string& label, float* value, float minValue, float maxValue, ValueType valueType, float labelColumnWidth, float widgetOffset)
    {
        ImGui::PushID(label.c_str());   // 设置控件 ID

        float panelWidth = ImGui::GetWindowContentRegionWidth();    // 面板宽度

        ImGui::Columns(2, 0, false);                // 设置为 两列 id 边界取消显示
        ImGui::SetColumnWidth(0, labelColumnWidth); // 设置 0 号列宽

        ImGui::Text(label.c_str());     // 控件名（0 号列）

        ImGui::NextColumn();

        ImGui::PushItemWidth(panelWidth - labelColumnWidth - widgetOffset); // 设置 1 号列宽 = 面板宽 - 标签列宽 - 小部件右边界向左偏移量

        // 滑动条（1 号列）
        switch (valueType)
        {
            case ValueType::Float:
                ImGui::SliderFloat("##Float", value, minValue, maxValue);
                break;
            case ValueType::Angle:
                ImGui::SliderAngle("##Angle", value, minValue, maxValue);   // 弧度
                break;
        }
        ImGui::PopItemWidth();

        ImGui::Columns(1);  // 设置为一列

        ImGui::PopID();
    }

    void GUI::CheckBox(const std::string& label, bool* value, float labelColumnWidth, float widgetOffset)
    {
        ImGui::PushID(label.c_str());   // 设置控件ID

        float panelWidth = ImGui::GetWindowContentRegionWidth();    // 面板宽度

        ImGui::Columns(2, 0, false);                // 设置为 两列 id 边界取消显示
        ImGui::SetColumnWidth(0, labelColumnWidth); // 设置 0 号列宽

        ImGui::Text(label.c_str());     //控件名（0 号列）

        ImGui::NextColumn();

        ImGui::PushItemWidth(panelWidth - labelColumnWidth - widgetOffset); // 设置 1 号列宽 = 面板宽 - 标签列宽 - 小部件右边界向左偏移量
        ImGui::Checkbox("##None", value);   // 勾选框（1 号列）
        ImGui::PopItemWidth();

        ImGui::Columns(1);      // 设置为一列

        ImGui::PopID();
    }
}
