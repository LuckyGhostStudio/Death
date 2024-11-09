#pragma once

#include <glm/glm.hpp>

namespace Lucky
{
    /*
    * TODO List:
    * 滚动条：单独的面板，在 Panel 上层，出现和消失不会影响 Panel 内的控件和布局
    * TreeNode：场景树和文件树需要用的
    * TreeNodeOther：其他控件需要用的
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
        /// Vector3 Drag 控件
        /// </summary>
        /// <param name="label">标签</param>
        /// <param name="values">值</param>
        /// <param name="resetValue">重置值</param>
        /// <param name="columnWidth">每列宽度</param>
        static void DragVector3(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
   
        /// <summary>
        /// Drag FloatN
        /// </summary>
        /// <param name="label">标签名</param>
        /// <param name="value">值</param>
        /// <param name="valueSpeed">拖动精度</param>
        /// <param name="valueType">值类型</param>
        /// <param name="minValue">最小值</param>
        /// <param name="maxValue">最大值</param>
        /// <param name="labelColumnWidth">标签列宽</param>
        /// <param name="widgetOffset">小部件右边界向左偏移量</param>
        static void DragFloatN(const std::string& label, float* value, float valueSpeed = 0.01f, ValueType valueType = ValueType::Float, float minValue = 0.0f, float maxValue = 0.0f, float labelColumnWidth = 120.0f, float widgetOffset = 30.0f);
    
        /// <summary>
        /// Drag Slider
        /// </summary>
        /// <param name="label">标签名</param>
        /// <param name="value">值</param>
        /// <param name="minValue">最小值</param>
        /// <param name="maxValue">最大值</param>
        /// <param name="valueType">值类型</param>
        /// <param name="labelColumnWidth">标签列宽</param>
        /// <param name="widgetOffset">小部件右边界向左偏移量</param>
        static void DragSlider(const std::string& label, float* value, float minValue, float maxValue, ValueType valueType = ValueType::Float, float labelColumnWidth = 120.0f, float widgetOffset = 30.0f);
    
        /// <summary>
        /// 勾选框
        /// </summary>
        /// <param name="label">标签名</param>
        /// <param name="value">值</param>
        /// <param name="labelColumnWidth">标签列宽</param>
        /// <param name="widgetOffset">小部件右边界向左偏移量</param>
        static void CheckBox(const std::string& label, bool* value, float labelColumnWidth = 120.0f, float widgetOffset = 30.0f);
    };
}