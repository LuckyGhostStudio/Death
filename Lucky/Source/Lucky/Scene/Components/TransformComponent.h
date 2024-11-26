#pragma once

#include "Component.h"
#include "Lucky/Renderer/Transform.h"

namespace Lucky
{
    /// <summary>
    /// Transform �����Ĭ�������
    /// </summary>
    struct TransformComponent
    {
        ComponentType Type = ComponentType::Transform;

        Transform Transform;

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;

        TransformComponent(const glm::vec3& position)
            : Transform(position) {}
    };
}