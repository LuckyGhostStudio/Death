#pragma once

#include <glm/glm.hpp>

namespace Lucky
{
    /*
    * TODO List:
    * ����������������壬�� Panel �ϲ㣬���ֺ���ʧ����Ӱ�� Panel �ڵĿؼ��Ͳ���
    * TreeNode�����������ļ�����Ҫ�õ�
    * TreeNodeOther�������ؼ���Ҫ�õ�
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
        /// Vector3 Drag �ؼ�
        /// </summary>
        /// <param name="label">��ǩ</param>
        /// <param name="values">ֵ</param>
        /// <param name="resetValue">����ֵ</param>
        /// <param name="columnWidth">ÿ�п��</param>
        static void DragVector3(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
   
        /// <summary>
        /// Drag FloatN
        /// </summary>
        /// <param name="label">��ǩ��</param>
        /// <param name="value">ֵ</param>
        /// <param name="valueSpeed">�϶�����</param>
        /// <param name="valueType">ֵ����</param>
        /// <param name="minValue">��Сֵ</param>
        /// <param name="maxValue">���ֵ</param>
        /// <param name="labelColumnWidth">��ǩ�п�</param>
        /// <param name="widgetOffset">С�����ұ߽�����ƫ����</param>
        static void DragFloatN(const std::string& label, float* value, float valueSpeed = 0.01f, ValueType valueType = ValueType::Float, float minValue = 0.0f, float maxValue = 0.0f, float labelColumnWidth = 120.0f, float widgetOffset = 30.0f);
    
        /// <summary>
        /// Drag Slider
        /// </summary>
        /// <param name="label">��ǩ��</param>
        /// <param name="value">ֵ</param>
        /// <param name="minValue">��Сֵ</param>
        /// <param name="maxValue">���ֵ</param>
        /// <param name="valueType">ֵ����</param>
        /// <param name="labelColumnWidth">��ǩ�п�</param>
        /// <param name="widgetOffset">С�����ұ߽�����ƫ����</param>
        static void DragSlider(const std::string& label, float* value, float minValue, float maxValue, ValueType valueType = ValueType::Float, float labelColumnWidth = 120.0f, float widgetOffset = 30.0f);
    
        /// <summary>
        /// ��ѡ��
        /// </summary>
        /// <param name="label">��ǩ��</param>
        /// <param name="value">ֵ</param>
        /// <param name="labelColumnWidth">��ǩ�п�</param>
        /// <param name="widgetOffset">С�����ұ߽�����ƫ����</param>
        static void CheckBox(const std::string& label, bool* value, float labelColumnWidth = 120.0f, float widgetOffset = 30.0f);
    };
}