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
    * TreeNode：场景树和文件树需要用的
    * TreeNodeOther：类似组件的树节点
    * 对象选择器框：用于显示当前的内置对象或资产
    */

    /// <summary>
    /// 编辑器 GUI
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
        /// 可选择下拉列表
        /// </summary>
        /// <param name="label">标签名</param>
        /// <param name="options">选项列表</param>
        /// <param name="optionCount">选项个数</param>
        /// <param name="selected">选中项下标</param>
        /// <param name="labelMinWidth">标签最小列宽</param>
        /// <param name="widgetOffset">小部件右边界向左偏移量</param>
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
        ImGui::PushID(label.c_str());   // 设置控件 ID

        float lineHeight = ImGui::GetIO().Fonts->Fonts[1]->FontSize;
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


        // 按钮（1 号列）
        float posX = ImGui::GetCursorPosX();
        if (ImGui::Button("##Name", ImVec2(panelWidth - labelWidth - widgetOffset, 0)))
        {
            OnClicked();   // 点击回调函数
        }

        // 对象图标
        ImGui::SameLine();
        ImGui::SetCursorPos({ posX + padding * 1.5f, ImGui::GetCursorPosY() + padding * 1.6f });
        ImGui::Image((ImTextureID)textureID, ImVec2(lineHeight - padding, lineHeight - padding), ImVec2(0, 1), ImVec2(1, 0));

        // 对象名 TODO None (ObjectType 模板参数)
        ImGui::SameLine();
        ImGui::SetCursorPos({ ImGui::GetCursorPosX() - padding, ImGui::GetCursorPosY() - padding * 1.6f });
        ImGui::Text(objectName.empty() ? "(None)" : objectName.c_str());

        ImGui::PopItemWidth();
        ImGui::Columns(1);      // 设置为一列

        ImGui::PopID();
    }

    template<typename T, typename Func>
    inline void GUI::Foldout(const std::string& label, uint32_t iconID, Func OnOpened, bool defaultOpen)
    {
        ImGui::PushID(label.c_str());   // 设置控件ID

        // 树结点标志
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (defaultOpen)
        {
            flags |= ImGuiTreeNodeFlags_DefaultOpen;
        }

        ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();     // 可用区域大小

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 1));   // Separator 和 TreeNodeEx 之间的垂直距离为 1
        ImGui::Separator(); // 分隔符
        ImGui::Separator(); // 分隔符

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);            // 边框圆度
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);          // 边框线宽 0

        ImFont* boldFont = ImGui::GetIO().Fonts->Fonts[0];  // 粗体

        float lineHeight = boldFont->FontSize;
        float padding = ImGui::GetStyle().FramePadding.y;

        ImVec2 nodePos = ImGui::GetCursorPos();

        static ImVec4 headerColor;

        // 哈希值作为结点 id
        bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, "");

        if (ImGui::IsItemHovered())
        {
            headerColor = ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered];
        }
        else
        {
            headerColor = ImGui::GetStyle().Colors[ImGuiCol_Header];
        }

        // 组件 Icon
        ImGui::SameLine();
        ImGui::SetCursorPosY(nodePos.y + padding * 1.5);
        ImGui::PushFont(boldFont);
        ImGui::Image((void*)iconID, ImVec2(lineHeight - padding, lineHeight - padding), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::PopFont();

        // 组件名字文本
        ImGui::SameLine(nodePos.x + lineHeight * 2.6);
        ImGui::SetCursorPosY(nodePos.y);
        ImGui::PushFont(boldFont);
        ImGui::Text(label.c_str());
        ImGui::PopFont();
        
        //ImGui::SameLine(contentRegionAvail.x - lineHeight - padding * 2);      // 同一行：可用区域左移
        //ImGui::SetCursorPosY(nodePos.y + padding);
        //// 组件设置按钮
        //ImGui::PushStyleColor(ImGuiCol_Button, headerColor);
        //if (ImGui::ImageButton((void*)m_SettingsButtonIcon->GetRendererID(), { lineHeight - padding * 2, lineHeight - padding * 2 }, ImVec2(0, 1), ImVec2(1, 0)))
        //{
        //    ImGui::OpenPopup("Settings");   // 打开弹出框
        //}
        //ImGui::PopStyleColor();

        ImGui::PopStyleVar(4);
        /*
        // 渲染弹出框
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