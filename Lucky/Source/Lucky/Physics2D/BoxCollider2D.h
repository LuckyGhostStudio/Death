#pragma once

#include <glm/glm.hpp>

namespace Lucky
{
    /// <summary>
    /// Box2D 碰撞体
    /// </summary>
    struct BoxCollider2D
    {
        glm::vec2 Offset = { 0.0f, 0.0f };
        glm::vec2 Size = { 1.0f, 1.0f };

        // TODO 移动到 Physics Material -> Asset
        float Density = 1.0f;               // 密度	kg / m ^ 2
        float Friction = 0.5f;              // 摩擦力[0, 1]
        float Restitution = 0.0f;           // 恢复系数：为0则没有弹性[0, 1]
        float RestitutionThreshold = 0.5f;  // 恢复阈值：速度低于此值则停止弹力计算

        void* RuntimeFixture = nullptr;     // 存储运行时数据

        BoxCollider2D() = default;
        BoxCollider2D(const BoxCollider2D&) = default;
    };
}