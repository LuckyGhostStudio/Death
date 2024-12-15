#include "lcpch.h"
#include "GUI.h"

#include <imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

namespace Lucky
{
    void GUI::DragFloat3(const std::string& label, glm::vec3& values, float valueSpeed, float minValue, float maxValue, float labelMinWidth, float widgetOffset)
    {
        ImGui::PushID(label.c_str());   // 设置控件 ID

        ImFont* boldFont = ImGui::GetIO().Fonts->Fonts[0];  // 粗体
        float padding = ImGui::GetStyle().FramePadding.y;

        float panelWidth = ImGui::GetWindowContentRegionWidth();    // 面板宽度
        // 计算 label 宽度 [labelMinWidth, panelWidth * 0.4f]
        float labelWidth = panelWidth * 0.4f;
        if (labelWidth < labelMinWidth)
        {
            labelWidth = labelMinWidth;
        }

        ImGui::Columns(2, 0, false);            // 设置为 两列 id 边界取消显示
        ImGui::SetColumnWidth(0, labelWidth);   // 设置 0 号列宽

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding);
        ImGui::Text(label.c_str()); // 控件名（0 号列）

        ImGui::NextColumn();
        ImGui::PushMultiItemsWidths(3, panelWidth - labelWidth - widgetOffset); // 设置 1 号列宽 = 面板宽 - 标签列宽 - 小部件右边界向左偏移量

        // 拖动条（1 号列）
        ImGui::PushFont(boldFont);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding);
        ImGui::Text("X");
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - padding);
        ImGui::DragFloat("##X", &values.x, valueSpeed, minValue, maxValue);

        ImGui::PopItemWidth();

        ImGui::SameLine();

        // 拖动条（2 号列）
        ImGui::PushFont(boldFont);
        ImGui::Text("Y");
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, valueSpeed, minValue, maxValue);

        ImGui::PopItemWidth();

        ImGui::SameLine();

        // 拖动条（3 号列）
        ImGui::PushFont(boldFont);
        ImGui::Text("Z");
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &values.z, valueSpeed, minValue, maxValue);

        ImGui::PopItemWidth();

        ImGui::Columns(1);  // 设置为一列

        ImGui::PopID();
    }

    bool GUI::DragFloatN(const std::string& label, float* value, float valueSpeed, ValueType valueType, float minValue, float maxValue, float labelMinWidth, float widgetOffset)
    {
        ImGui::PushID(label.c_str());   // 设置控件 ID

        float padding = ImGui::GetStyle().FramePadding.y;

        float panelWidth = ImGui::GetWindowContentRegionWidth();    // 面板宽度
        // 计算 label 宽度 [labelMinWidth, panelWidth * 0.4f]
        float labelWidth = panelWidth * 0.4f;
        if (labelWidth < labelMinWidth)
        {
            labelWidth = labelMinWidth;
        }  

        ImGui::Columns(2, 0, false);            // 设置为 两列 id 边界取消显示
        ImGui::SetColumnWidth(0, labelWidth);   // 设置 0 号列宽

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding);
        ImGui::Text(label.c_str()); // 控件名（0 号列）

        ImGui::NextColumn();
        ImGui::PushItemWidth(panelWidth - labelWidth - widgetOffset); // 设置 1 号列宽 = 面板宽 - 标签列宽 - 小部件右边界向左偏移量

        bool changed = false;

        // 拖动条（1 号列）
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
        ImGui::Columns(1);  // 设置为一列

        ImGui::PopID();

        return changed;
    }

    void GUI::DragSlider(const std::string& label, float* value, float minValue, float maxValue, ValueType valueType, float labelMinWidth, float widgetOffset)
    {
        ImGui::PushID(label.c_str());   // 设置控件 ID

        float padding = ImGui::GetStyle().FramePadding.y;

        float panelWidth = ImGui::GetWindowContentRegionWidth();    // 面板宽度
        // 计算 label 宽度 [labelMinWidth, panelWidth * 0.4f]
        float labelWidth = panelWidth * 0.4f;
        if (labelWidth < labelMinWidth)
        {
            labelWidth = labelMinWidth;
        }

        ImGui::Columns(2, 0, false);            // 设置为 两列 id 边界取消显示
        ImGui::SetColumnWidth(0, labelWidth);   // 设置 0 号列宽

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding);
        ImGui::Text(label.c_str()); // 控件名（0 号列）

        ImGui::NextColumn();
        ImGui::PushItemWidth(panelWidth - labelWidth - widgetOffset); // 设置 1 号列宽 = 面板宽 - 标签列宽 - 小部件右边界向左偏移量

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

    void GUI::CheckBox(const std::string& label, bool* value, float labelMinWidth, float widgetOffset)
    {
        ImGui::PushID(label.c_str());   // 设置控件ID

        float padding = ImGui::GetStyle().FramePadding.y;

        float panelWidth = ImGui::GetWindowContentRegionWidth();    // 面板宽度
        // 计算 label 宽度 [labelMinWidth, panelWidth * 0.4f]
        float labelWidth = panelWidth * 0.4f;
        if (labelWidth < labelMinWidth)
        {
            labelWidth = labelMinWidth;
        }

        ImGui::Columns(2, 0, false);            // 设置为 两列 id 边界取消显示
        ImGui::SetColumnWidth(0, labelWidth);   // 设置 0 号列宽

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding);
        ImGui::Text(label.c_str()); // 控件名（0 号列）

        ImGui::NextColumn();
        ImGui::PushItemWidth(panelWidth - labelWidth - widgetOffset); // 设置 1 号列宽 = 面板宽 - 标签列宽 - 小部件右边界向左偏移量

        ImGui::Checkbox("##None", value);   // 勾选框（1 号列）
        
        ImGui::PopItemWidth();
        ImGui::Columns(1);      // 设置为一列

        ImGui::PopID();
    }

    void GUI::ColorEditor4(const std::string& label, glm::vec4& color, float labelMinWidth, float widgetOffset)
    {
        ImGui::PushID(label.c_str());   // 设置控件ID
#if 1   // TODO 移动到通用
        float padding = ImGui::GetStyle().FramePadding.y;

        float panelWidth = ImGui::GetWindowContentRegionWidth();    // 面板宽度
        // 计算 label 宽度 [labelMinWidth, panelWidth * 0.4f]
        float labelWidth = panelWidth * 0.4f;
        if (labelWidth < labelMinWidth)
        {
            labelWidth = labelMinWidth;
        }

        ImGui::Columns(2, 0, false);            // 设置为 两列 id 边界取消显示
        ImGui::SetColumnWidth(0, labelWidth);   // 设置 0 号列宽

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding);
        ImGui::Text(label.c_str()); // 控件名（0 号列）

        ImGui::NextColumn();
        ImGui::PushItemWidth(panelWidth - labelWidth - widgetOffset); // 设置 1 号列宽 = 面板宽 - 标签列宽 - 小部件右边界向左偏移量
#endif
        ImGui::ColorEdit4("##None", glm::value_ptr(color)); // 颜色编辑器（1号列）

        ImGui::PopItemWidth();
        ImGui::Columns(1);      // 设置为一列

        ImGui::PopID();
    }
}
