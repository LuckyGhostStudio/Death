#pragma once

#include "Component.h"
#include "Lucky/Physics2D/CircleCollider2D.h"

namespace Lucky
{
    /// <summary>
    /// 2D ¸ÕÌå×é¼þ
    /// </summary>
    struct CircleCollider2DComponent
    {
        ComponentType Type = ComponentType::CircleCollider2D;

        CircleCollider2D CircleCollider2d;

        CircleCollider2DComponent() = default;
        CircleCollider2DComponent(const CircleCollider2DComponent&) = default;
        CircleCollider2DComponent(const CircleCollider2D& circleCollider2d)
            : CircleCollider2d(circleCollider2d)
        {

        }
    };
}