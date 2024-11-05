#pragma once

#include <glm/glm.hpp>

namespace Lucky
{
    class GUI
    {
    public:
        /// <summary>
        /// Vector3 Drag �ؼ�
        /// </summary>
        /// <param name="label">��ǩ</param>
        /// <param name="values">ֵ</param>
        /// <param name="resetValue">����ֵ</param>
        /// <param name="columnWidth">ÿ�п��</param>
        static void DragVector3(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
    };
}