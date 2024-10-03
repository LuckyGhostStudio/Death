#pragma once

#include "Component.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Lucky
{
    /// <summary>
    /// Transform �����Ĭ�������
    /// </summary>
    struct TransformComponent : public Component
    {
        glm::vec3 Position = { 0.0f, 0.0f, 0.0f };  // λ��
        glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };  // ��ת
        glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };     // ����

        TransformComponent()
            : Component(ComponentName::Transform, true) {}

        TransformComponent(const glm::vec3& position)
            : Component(ComponentName::Transform, true), Position(position) {}

        TransformComponent(const TransformComponent&) = default;

        /// <summary>
        /// ���� Transform ����
        /// </summary>
        /// <returns>Transform ����</returns>
        glm::mat4 GetTransform() const
        {
            glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(Rotation)));    // ��ת������Ԫ����ת

            return glm::translate(glm::mat4(1.0f), Position) * rotation * glm::scale(glm::mat4(1.0f), Scale);
        }

        /// <summary>
        /// Right ���� x+
        /// </summary>
        /// <returns>Right ����</returns>
        glm::vec3 GetRightDirection() const
        {
            return glm::normalize(glm::rotate(glm::quat(glm::radians(Rotation)), glm::vec3(1.0f, 0.0f, 0.0f)));
        }

        /// <summary>
        /// Up ���� y+
        /// </summary>
        /// <returns>Up ����</returns>
        glm::vec3 GetUpDirection() const
        {
            return glm::normalize(glm::rotate(glm::quat(glm::radians(Rotation)), glm::vec3(0.0f, 1.0f, 0.0f)));
        }

        /// <summary>
        /// Forward ���� z+
        /// </summary>
        /// <returns>Forward ����</returns>
        glm::vec3 GetForwardDirection() const
        {
            return glm::normalize(glm::rotate(glm::quat(glm::radians(Rotation)), glm::vec3(0.0f, 0.0f, 1.0f)));
        }
    };
}