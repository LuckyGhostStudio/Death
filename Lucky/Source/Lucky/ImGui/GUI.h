#pragma once

#include <glm/glm.hpp>

namespace Lucky
{
    /*
    * TODO List:
    * TreeNode�����������ļ�����Ҫ�õ�
    * TreeNodeOther��������������ڵ�
    */

    /// <summary>
    /// �༭�� GUI
    /// </summary>
    class GUI
    {
    public:
        /// <summary>
        /// ֵ����
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
        /// <param name="label">��ǩ��</param>
        /// <param name="value">ֵ</param>
        /// <param name="valueSpeed">�϶�����</param>
        /// <param name="minValue">��Сֵ</param>
        /// <param name="maxValue">���ֵ</param>
        /// <param name="labelMinWidth">��ǩ��С�п�</param>
        /// <param name="widgetOffset">С�����ұ߽�����ƫ����</param>
        static void DragFloat3(const std::string& label, glm::vec3& values, float valueSpeed = 0.01f, float minValue = 0.0f, float maxValue = 0.0f, float labelMinWidth = 120.0f, float widgetOffset = 105.0f);
   
        /// <summary>
        /// Drag FloatN
        /// </summary>
        /// <param name="label">��ǩ��</param>
        /// <param name="value">ֵ</param>
        /// <param name="valueSpeed">�϶�����</param>
        /// <param name="valueType">ֵ����</param>
        /// <param name="minValue">��Сֵ</param>
        /// <param name="maxValue">���ֵ</param>
        /// <param name="labelMinWidth">��ǩ��С�п�</param>
        /// <param name="widgetOffset">С�����ұ߽�����ƫ����</param>
        static void DragFloatN(const std::string& label, float* value, float valueSpeed = 0.01f, ValueType valueType = ValueType::Float, float minValue = 0.0f, float maxValue = 0.0f, float labelMinWidth = 120.0f, float widgetOffset = 40.0f);
    
        /// <summary>
        /// Drag Slider
        /// </summary>
        /// <param name="label">��ǩ��</param>
        /// <param name="value">ֵ</param>
        /// <param name="minValue">��Сֵ</param>
        /// <param name="maxValue">���ֵ</param>
        /// <param name="valueType">ֵ����</param>
        /// <param name="labelMinWidth">��ǩ��С�п�</param>
        /// <param name="widgetOffset">С�����ұ߽�����ƫ����</param>
        static void DragSlider(const std::string& label, float* value, float minValue, float maxValue, ValueType valueType = ValueType::Float, float labelMinWidth = 120.0f, float widgetOffset = 40.0f);
    
        /// <summary>
        /// ��ѡ��
        /// </summary>
        /// <param name="label">��ǩ��</param>
        /// <param name="value">ֵ</param>
        /// <param name="labelMinWidth">��ǩ��С�п�</param>
        /// <param name="widgetOffset">С�����ұ߽�����ƫ����</param>
        static void CheckBox(const std::string& label, bool* value, float labelMinWidth = 120.0f, float widgetOffset = 40.0f);
    };
}