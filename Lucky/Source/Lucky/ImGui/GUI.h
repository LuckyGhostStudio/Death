#pragma once

#include <glm/glm.hpp>

namespace Lucky
{
    class GUI
    {
    public:
        /// <summary>
        /// Vector3 Drag 控件
        /// </summary>
        /// <param name="label">标签</param>
        /// <param name="values">值</param>
        /// <param name="resetValue">重置值</param>
        /// <param name="columnWidth">每列宽度</param>
        static void DragVector3(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
    };
}