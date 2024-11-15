#pragma once

#include <imgui/imgui.h>
#include <glm/glm.hpp>

namespace Lucky
{
    /*
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
        /// <summary>
        /// 值类型
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
        /// <param name="label">标签名</param>
        /// <param name="value">值</param>
        /// <param name="valueSpeed">拖动精度</param>
        /// <param name="minValue">最小值</param>
        /// <param name="maxValue">最大值</param>
        /// <param name="labelMinWidth">标签最小列宽</param>
        /// <param name="widgetOffset">小部件右边界向左偏移量</param>
        static void DragFloat3(const std::string& label, glm::vec3& values, float valueSpeed = 0.01f, float minValue = 0.0f, float maxValue = 0.0f, float labelMinWidth = 120.0f, float widgetOffset = 105.0f);
   
        /// <summary>
        /// Drag FloatN
        /// </summary>
        /// <param name="label">标签名</param>
        /// <param name="value">值</param>
        /// <param name="valueSpeed">拖动精度</param>
        /// <param name="valueType">值类型</param>
        /// <param name="minValue">最小值</param>
        /// <param name="maxValue">最大值</param>
        /// <param name="labelMinWidth">标签最小列宽</param>
        /// <param name="widgetOffset">小部件右边界向左偏移量</param>
        static void DragFloatN(const std::string& label, float* value, float valueSpeed = 0.01f, ValueType valueType = ValueType::Float, float minValue = 0.0f, float maxValue = 0.0f, float labelMinWidth = 120.0f, float widgetOffset = 40.0f);
    
        /// <summary>
        /// Drag Slider
        /// </summary>
        /// <param name="label">标签名</param>
        /// <param name="value">值</param>
        /// <param name="minValue">最小值</param>
        /// <param name="maxValue">最大值</param>
        /// <param name="valueType">值类型</param>
        /// <param name="labelMinWidth">标签最小列宽</param>
        /// <param name="widgetOffset">小部件右边界向左偏移量</param>
        static void DragSlider(const std::string& label, float* value, float minValue, float maxValue, ValueType valueType = ValueType::Float, float labelMinWidth = 120.0f, float widgetOffset = 40.0f);
    
        /// <summary>
        /// 勾选框
        /// </summary>
        /// <param name="label">标签名</param>
        /// <param name="value">值</param>
        /// <param name="labelMinWidth">标签最小列宽</param>
        /// <param name="widgetOffset">小部件右边界向左偏移量</param>
        static void CheckBox(const std::string& label, bool* value, float labelMinWidth = 120.0f, float widgetOffset = 40.0f);

        /// <summary>
        /// 颜色编辑器
        /// </summary>
        /// <param name="label">标签名</param>
        /// <param name="color">颜色值</param>
        /// <param name="labelMinWidth"></param>
        /// <param name="widgetOffset"></param>
        static void ColorEditor4(const std::string& label, glm::vec4& color, float labelMinWidth = 120.0f, float widgetOffset = 40.0f);

        /// <summary>
        /// 可选择下拉列表
        /// </summary>
        /// <typeparam name="Func">某项选中时回调函数</typeparam>
        /// <param name="label">标签名</param>
        /// <param name="currentValueStr">当前值</param>
        /// <param name="valueStrs">所有可选值列表</param>
        /// <param name="valueStrCount">值个数</param>
        /// <param name="OnSelected">某项选中时回调函数：参数（int index 项编号，const char* value 项值）</param>
        /// <param name="labelMinWidth">标签最小列宽</param>
        /// <param name="widgetOffset">小部件右边界向左偏移量</param>
        template<typename Func>
        inline static void DropdownList(const std::string& label, const char* currentValueStr, const char* valueStrs[], uint32_t valueStrCount, Func OnSelected, float labelMinWidth = 120.0f, float widgetOffset = 40.0f);

        template<typename Func>
        inline static void ObjectSelector(const std::string& label, uint32_t textureID, const glm::vec2& size, Func OnSelected, float framePadding = 2.0f, float labelMinWidth = 120.0f, float widgetOffset = 40.0f);
    };

    template<typename Func>
    inline void GUI::DropdownList(const std::string& label, const char* currentValueStr, const char* valueStrs[], uint32_t valueStrCount, Func OnSelected, float labelMinWidth, float widgetOffset)
    {
        ImGui::PushID(label.c_str());   // 设置控件 ID

        float panelWidth = ImGui::GetWindowContentRegionWidth();    // 面板宽度
        // 计算 label 宽度 [labelMinWidth, panelWidth * 0.4f]
        float labelWidth = panelWidth * 0.4f;
        if (labelWidth < labelMinWidth)
        {
            labelWidth = labelMinWidth;
        }

        ImGui::Columns(2, 0, false);            // 设置为 两列 id 边界取消显示
        ImGui::SetColumnWidth(0, labelWidth);   // 设置 0 号列宽

        ImGui::Text(label.c_str()); // 控件名（0 号列）

        ImGui::NextColumn();
        ImGui::PushItemWidth(panelWidth - labelWidth - widgetOffset); // 设置 1 号列宽 = 面板宽 - 标签列宽 - 小部件右边界向左偏移量

        // 下拉框（1 号列）
        if (ImGui::BeginCombo("##None", currentValueStr))
        {
            // 查找选中项
            for (int i = 0; i < valueStrCount; i++)
            {
                bool isSelected = currentValueStr == valueStrs[i]; // 被选中：当前值==第i个值
                // 可选择项，该项改变时：第i项为选中项
                if (ImGui::Selectable(valueStrs[i], isSelected))
                {
                    currentValueStr = valueStrs[i]; // 设置当前值
                    OnSelected(i, currentValueStr); // i 项选中事件函数
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

        ImGui::PopID();
    }

    template<typename Func>
    inline void GUI::ObjectSelector(const std::string& label, uint32_t textureID, const glm::vec2& size, Func OnSelected, float framePadding, float labelMinWidth, float widgetOffset)
    {
        ImGui::PushID(label.c_str());   // 设置控件 ID

        float panelWidth = ImGui::GetWindowContentRegionWidth();    // 面板宽度
        // 计算 label 宽度 [labelMinWidth, panelWidth * 0.4f]
        float labelWidth = panelWidth * 0.4f;
        if (labelWidth < labelMinWidth)
        {
            labelWidth = labelMinWidth;
        }

        ImGui::Columns(2, 0, false);            // 设置为 两列 id 边界取消显示
        ImGui::SetColumnWidth(0, labelWidth);   // 设置 0 号列宽

        ImGui::Text(label.c_str()); // 控件名（0 号列）

        ImGui::NextColumn();
        ImGui::PushItemWidth(panelWidth - labelWidth - widgetOffset); // 设置 1 号列宽 = 面板宽 - 标签列宽 - 小部件右边界向左偏移量

        // TODO 点击进行资源定位 OnLocated
        // Icon
        // Text

        // TODO 点击进行资源或对象选择
        // 按钮（1 号列）
        if (ImGui::ImageButton((ImTextureID)textureID, ImVec2(size.x, size.y), ImVec2(0, 1), ImVec2(1, 0), framePadding))
        {
            OnSelected();   // 调用资源/对象选择回调函数
        }

        ImGui::PopItemWidth();
        ImGui::Columns(1);      // 设置为一列

        ImGui::PopID();
    }
}