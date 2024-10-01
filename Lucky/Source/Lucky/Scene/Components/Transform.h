#pragma once

#include <glm/glm.hpp>

namespace Lucky
{
    /// <summary>
    /// Transform 组件
    /// </summary>
    class Transform
    {
    private:
        glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };   // 位置
        glm::vec3 m_Rotation{ 0.0f, 0.0f, 0.0f };   // 旋转
        glm::vec3 m_Scale{ 1.0f, 1.0f, 1.0f };      // 缩放

        glm::mat4 m_Transform{ 1.0f };
    public:
        Transform() = default;
        Transform(const Transform&) = default;
        Transform(const glm::mat4& transform) : m_Transform(transform) {}

        operator glm::mat4& () { return m_Transform; }
        operator const glm::mat4& () const { return m_Transform; }

        glm::vec3& GetPosition() { return m_Position; }
        glm::vec3& GetRotation() { return m_Rotation; }
        glm::vec3& GetScale() { return m_Scale; }
    };
}