#include "lcpch.h"
#include "GUI.h"

#include <imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

namespace Lucky
{
    bool GUI::DragFloat(const std::string& label, float* value, float valueSpeed, float minValue, float maxValue, float labelMinWidth, float widgetOffset)
    {
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

        bool changed = ImGui::DragFloat(std::format("##{0}", label).c_str(), value, valueSpeed, minValue, maxValue);

        ImGui::PopItemWidth();
        ImGui::Columns(1);  // 设置为一列

        return changed;
    }

    bool GUI::DragFloat2(const std::string& label, glm::vec2& values, float valueSpeed, float minValue, float maxValue, float labelMinWidth, float widgetOffset)
    {
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
        ImGui::PushMultiItemsWidths(2, panelWidth - labelWidth - widgetOffset); // 设置 1 号列宽 = 面板宽 - 标签列宽 - 小部件右边界向左偏移量

        // 拖动条（1 号列）
        ImGui::PushFont(boldFont);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding);
        ImGui::Text("X");
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - padding);
        bool changed = ImGui::DragFloat(std::format("##{0}X", label).c_str(), &values.x, valueSpeed, minValue, maxValue);

        ImGui::PopItemWidth();

        ImGui::SameLine();

        // 拖动条（2 号列）
        ImGui::PushFont(boldFont);
        ImGui::Text("Y");
        ImGui::PopFont();

        ImGui::SameLine();
        changed |= ImGui::DragFloat(std::format("##{0}Y", label).c_str(), &values.y, valueSpeed, minValue, maxValue);

        ImGui::PopItemWidth();

        ImGui::Columns(1);  // 设置为一列

        return changed;
    }

    bool GUI::DragFloat3(const std::string& label, glm::vec3& values, float valueSpeed, float minValue, float maxValue, float labelMinWidth, float widgetOffset)
    {
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
        bool changed = ImGui::DragFloat(std::format("##{0}X", label).c_str(), &values.x, valueSpeed, minValue, maxValue);

        ImGui::PopItemWidth();

        ImGui::SameLine();

        // 拖动条（2 号列）
        ImGui::PushFont(boldFont);
        ImGui::Text("Y");
        ImGui::PopFont();

        ImGui::SameLine();
        changed |= ImGui::DragFloat(std::format("##{0}Y", label).c_str(), &values.y, valueSpeed, minValue, maxValue);

        ImGui::PopItemWidth();

        ImGui::SameLine();

        // 拖动条（3 号列）
        ImGui::PushFont(boldFont);
        ImGui::Text("Z");
        ImGui::PopFont();

        ImGui::SameLine();
        changed |= ImGui::DragFloat(std::format("##{0}Z", label).c_str(), &values.z, valueSpeed, minValue, maxValue);

        ImGui::PopItemWidth();

        ImGui::Columns(1);  // 设置为一列

        return changed;
    }

    bool GUI::SliderFloat(const std::string& label, float* value, float minValue, float maxValue, float labelMinWidth, float widgetOffset)
    {
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
        bool changed = ImGui::SliderFloat(std::format("##{0}", label).c_str(), value, minValue, maxValue);

        ImGui::PopItemWidth();
        ImGui::Columns(1);  // 设置为一列

        return changed;
    }

    bool GUI::SliderAngle(const std::string& label, float* value, float minValue, float maxValue, float labelMinWidth, float widgetOffset)
    {
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
        bool changed = ImGui::SliderAngle(std::format("##{0}", label).c_str(), value, minValue, maxValue);

        ImGui::PopItemWidth();
        ImGui::Columns(1);  // 设置为一列

        return changed;
    }

    bool GUI::Toggle(const std::string& label, bool* value, float labelMinWidth, float widgetOffset)
    {
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

        // 勾选框（1 号列）
        bool changed = ImGui::Checkbox(std::format("##{0}", label).c_str(), value);
        
        ImGui::PopItemWidth();
        ImGui::Columns(1);      // 设置为一列

        return changed;
    }

    bool GUI::ColorEditor4(const std::string& label, glm::vec4& color, float labelMinWidth, float widgetOffset)
    {
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
        bool changed = ImGui::ColorEdit4(std::format("##{0}", label).c_str(), glm::value_ptr(color));   // 颜色编辑器（1号列）

        ImGui::PopItemWidth();
        ImGui::Columns(1);      // 设置为一列

        return changed;
    }

    bool GUI::DropdownList(const std::string& label, const char** options, uint32_t optionCount, int* selected, float labelMinWidth, float widgetOffset)
    {
        const char* current = options[*selected];   // 当前选中项

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

        // 下拉框（1 号列）
        if (ImGui::BeginCombo(std::format("##{0}", label).c_str(), current))
        {
            // 查找选中项
            for (int i = 0; i < optionCount; i++)
            {
                const bool isSelected = (current == options[i]); // 被选中：当前值==第i个值
                // 可选择项，该项改变时：第i项为选中项
                if (ImGui::Selectable(options[i], isSelected))
                {
                    current = options[i];   // 设置当前值
                    *selected = i;
                    changed = true;
                }

                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();   // 设置默认选中项
                }
            }
            ImGui::EndCombo();
        }

        ImGui::PopItemWidth();
        ImGui::Columns(1);      // 设置为一列

        return changed;
    }

    bool GUI::ImageButton(const std::string& label, uint32_t textureID, const glm::vec2& size, float framePadding, float labelMinWidth, float widgetOffset)
    {
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

        // 图片按钮（1 号列）
        bool changed = ImGui::ImageButton((ImTextureID)textureID, ImVec2(size.x, size.y), ImVec2(0, 1), ImVec2(1, 0), framePadding);

        // TODO 取消图片按钮

        ImGui::PopItemWidth();
        ImGui::Columns(1);      // 设置为一列

        return changed;
    }

    bool GUI::SelectableImageButton(uint32_t textureID, const glm::vec2& size, const glm::vec4 colors[4], float framePadding)
    {
        static bool selected = false;

        static ImVec4 buttonColor = { colors[0].r, colors[0].g, colors[0].b,colors[0].a, };         // 原色
        static ImVec4 buttonHoveredColor = { colors[1].r, colors[1].g, colors[1].b,colors[1].a, };  // 悬浮颜色
        static ImVec4 buttonActiveColor = { colors[2].r, colors[2].g, colors[2].b,colors[2].a, };   // 激活颜色

        if (selected)
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
        bool changed = ImGui::ImageButton((ImTextureID)textureID, ImVec2(size.x, size.y), ImVec2(0, 1), ImVec2(1, 0), framePadding);
        
        if (changed)
        {
            selected = !selected;
        }

        ImGui::PopStyleColor(3);

        return changed;
    }
}
