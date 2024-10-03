#pragma once

#include "Component.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Lucky
{
    /// <summary>
    /// Transform 组件（默认组件）
    /// </summary>
    struct TransformComponent : public Component
    {
        glm::vec3 Position = { 0.0f, 0.0f, 0.0f };  // 位置
        glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };  // 旋转
        glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };     // 缩放

        TransformComponent()
            : Component(ComponentName::Transform, true) {}

        TransformComponent(const glm::vec3& position)
            : Component(ComponentName::Transform, true), Position(position) {}

        TransformComponent(const TransformComponent&) = default;

        /// <summary>
        /// 返回 Transform 矩阵
        /// </summary>
        /// <returns>Transform 矩阵</returns>
        glm::mat4 GetTransform() const
        {
            glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(Rotation)));    // 旋转矩阵：四元数旋转

            return glm::translate(glm::mat4(1.0f), Position) * rotation * glm::scale(glm::mat4(1.0f), Scale);
        }

        /// <summary>
        /// Right 方向 x+
        /// </summary>
        /// <returns>Right 方向</returns>
        glm::vec3 GetRightDirection() const
        {
            return glm::normalize(glm::rotate(glm::quat(glm::radians(Rotation)), glm::vec3(1.0f, 0.0f, 0.0f)));
        }

        /// <summary>
        /// Up 方向 y+
        /// </summary>
        /// <returns>Up 方向</returns>
        glm::vec3 GetUpDirection() const
        {
            return glm::normalize(glm::rotate(glm::quat(glm::radians(Rotation)), glm::vec3(0.0f, 1.0f, 0.0f)));
        }

        /// <summary>
        /// Forward 方向 z+
        /// </summary>
        /// <returns>Forward 方向</returns>
        glm::vec3 GetForwardDirection() const
        {
            return glm::normalize(glm::rotate(glm::quat(glm::radians(Rotation)), glm::vec3(0.0f, 0.0f, 1.0f)));
        }
    };
}