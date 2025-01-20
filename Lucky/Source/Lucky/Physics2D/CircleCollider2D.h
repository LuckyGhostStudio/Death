#pragma once

#include <glm/glm.hpp>

namespace Lucky
{
    /// <summary>
    /// Circle2D 碰撞体
    /// </summary>
    struct CircleCollider2D
    {
        glm::vec2 Offset = { 0.0f, 0.0f };
        float Radius = 0.5f;

        // TODO 移动到 Physics Material
        float Density = 1.0f;               // 密度	kg / m ^ 2
        float Friction = 0.5f;              // 摩擦力[0, 1]
        float Restitution = 0.0f;           // 恢复系数：为0则没有弹性[0, 1]
        float RestitutionThreshold = 0.5f;  // 恢复阈值：速度低于此值则停止弹力计算

        void* RuntimeFixture = nullptr;     // 存储运行时数据

        CircleCollider2D() = default;
        CircleCollider2D(const CircleCollider2D&) = default;
    };
}