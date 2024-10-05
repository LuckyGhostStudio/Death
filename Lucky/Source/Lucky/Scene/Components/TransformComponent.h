#pragma once

#include "Component.h"

#include "Lucky/Renderer/Transform.h"

namespace Lucky
{
    /// <summary>
    /// Transform 组件（默认组件）
    /// </summary>
    struct TransformComponent : public Component
    {
        Transform Transform;

        TransformComponent()
            : Component(ComponentName::Transform, true) {}

        TransformComponent(const glm::vec3& position)
            : Component(ComponentName::Transform, true), Transform(position) {}

        TransformComponent(const TransformComponent&) = default;
    };
}