#include "lcpch.h"
#include "GUI.h"

#include <imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

namespace Lucky
{
    bool GUI::DragFloat(const std::string& label, float* value, float valueSpeed, float minValue, float maxValue, float labelMinWidth, float widgetOffset)
    {
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

        bool changed = ImGui::DragFloat(std::format("##{0}", label).c_str(), value, valueSpeed, minValue, maxValue);

        ImGui::PopItemWidth();
        ImGui::Columns(1);  // ����Ϊһ��

        return changed;
    }

    bool GUI::DragFloat2(const std::string& label, glm::vec2& values, float valueSpeed, float minValue, float maxValue, float labelMinWidth, float widgetOffset)
    {
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
        ImGui::PushMultiItemsWidths(2, panelWidth - labelWidth - widgetOffset); // ���� 1 ���п� = ���� - ��ǩ�п� - С�����ұ߽�����ƫ����

        // �϶�����1 ���У�
        ImGui::PushFont(boldFont);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding);
        ImGui::Text("X");
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - padding);
        bool changed = ImGui::DragFloat(std::format("##{0}X", label).c_str(), &values.x, valueSpeed, minValue, maxValue);

        ImGui::PopItemWidth();

        ImGui::SameLine();

        // �϶�����2 ���У�
        ImGui::PushFont(boldFont);
        ImGui::Text("Y");
        ImGui::PopFont();

        ImGui::SameLine();
        changed |= ImGui::DragFloat(std::format("##{0}Y", label).c_str(), &values.y, valueSpeed, minValue, maxValue);

        ImGui::PopItemWidth();

        ImGui::Columns(1);  // ����Ϊһ��

        return changed;
    }

    bool GUI::DragFloat3(const std::string& label, glm::vec3& values, float valueSpeed, float minValue, float maxValue, float labelMinWidth, float widgetOffset)
    {
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
        bool changed = ImGui::DragFloat(std::format("##{0}X", label).c_str(), &values.x, valueSpeed, minValue, maxValue);

        ImGui::PopItemWidth();

        ImGui::SameLine();

        // �϶�����2 ���У�
        ImGui::PushFont(boldFont);
        ImGui::Text("Y");
        ImGui::PopFont();

        ImGui::SameLine();
        changed |= ImGui::DragFloat(std::format("##{0}Y", label).c_str(), &values.y, valueSpeed, minValue, maxValue);

        ImGui::PopItemWidth();

        ImGui::SameLine();

        // �϶�����3 ���У�
        ImGui::PushFont(boldFont);
        ImGui::Text("Z");
        ImGui::PopFont();

        ImGui::SameLine();
        changed |= ImGui::DragFloat(std::format("##{0}Z", label).c_str(), &values.z, valueSpeed, minValue, maxValue);

        ImGui::PopItemWidth();

        ImGui::Columns(1);  // ����Ϊһ��

        return changed;
    }

    bool GUI::SliderFloat(const std::string& label, float* value, float minValue, float maxValue, float labelMinWidth, float widgetOffset)
    {
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
        bool changed = ImGui::SliderFloat(std::format("##{0}", label).c_str(), value, minValue, maxValue);

        ImGui::PopItemWidth();
        ImGui::Columns(1);  // ����Ϊһ��

        return changed;
    }

    bool GUI::SliderAngle(const std::string& label, float* value, float minValue, float maxValue, float labelMinWidth, float widgetOffset)
    {
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
        bool changed = ImGui::SliderAngle(std::format("##{0}", label).c_str(), value, minValue, maxValue);

        ImGui::PopItemWidth();
        ImGui::Columns(1);  // ����Ϊһ��

        return changed;
    }

    bool GUI::Toggle(const std::string& label, bool* value, float labelMinWidth, float widgetOffset)
    {
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

        // ��ѡ��1 ���У�
        bool changed = ImGui::Checkbox(std::format("##{0}", label).c_str(), value);
        
        ImGui::PopItemWidth();
        ImGui::Columns(1);      // ����Ϊһ��

        return changed;
    }

    bool GUI::ColorEditor4(const std::string& label, glm::vec4& color, float labelMinWidth, float widgetOffset)
    {
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
        bool changed = ImGui::ColorEdit4(std::format("##{0}", label).c_str(), glm::value_ptr(color));   // ��ɫ�༭����1���У�

        ImGui::PopItemWidth();
        ImGui::Columns(1);      // ����Ϊһ��

        return changed;
    }

    bool GUI::DropdownList(const std::string& label, const char** options, uint32_t optionCount, int* selected, float labelMinWidth, float widgetOffset)
    {
        const char* current = options[*selected];   // ��ǰѡ����

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

        // ������1 ���У�
        if (ImGui::BeginCombo(std::format("##{0}", label).c_str(), current))
        {
            // ����ѡ����
            for (int i = 0; i < optionCount; i++)
            {
                const bool isSelected = (current == options[i]); // ��ѡ�У���ǰֵ==��i��ֵ
                // ��ѡ�������ı�ʱ����i��Ϊѡ����
                if (ImGui::Selectable(options[i], isSelected))
                {
                    current = options[i];   // ���õ�ǰֵ
                    *selected = i;
                    changed = true;
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

        return changed;
    }

    bool GUI::ImageButton(const std::string& label, uint32_t textureID, const glm::vec2& size, float framePadding, float labelMinWidth, float widgetOffset)
    {
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

        // ͼƬ��ť��1 ���У�
        bool changed = ImGui::ImageButton((ImTextureID)textureID, ImVec2(size.x, size.y), ImVec2(0, 1), ImVec2(1, 0), framePadding);

        // TODO ȡ��ͼƬ��ť

        ImGui::PopItemWidth();
        ImGui::Columns(1);      // ����Ϊһ��

        return changed;
    }

    bool GUI::SelectableImageButton(uint32_t textureID, const glm::vec2& size, const glm::vec4 colors[4], float framePadding)
    {
        static bool selected = false;

        static ImVec4 buttonColor = { colors[0].r, colors[0].g, colors[0].b,colors[0].a, };         // ԭɫ
        static ImVec4 buttonHoveredColor = { colors[1].r, colors[1].g, colors[1].b,colors[1].a, };  // ������ɫ
        static ImVec4 buttonActiveColor = { colors[2].r, colors[2].g, colors[2].b,colors[2].a, };   // ������ɫ

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
