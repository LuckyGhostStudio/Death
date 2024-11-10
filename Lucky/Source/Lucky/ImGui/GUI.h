#pragma once

#include <glm/glm.hpp>

namespace Lucky
{
    /*
    * TODO List:
    * TreeNode：场景树和文件树需要用的
    * TreeNodeOther：类似组件的树节点
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
    };
}