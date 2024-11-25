#pragma once

#include "Component.h"
#include "Lucky/Physics2D/Rigidbody2D.h"

namespace Lucky
{
    /// <summary>
    /// 2D ¸ÕÌå×é¼þ
    /// </summary>
    struct Rigidbody2DComponent : public Component
    {
        Rigidbody2D Rigidbody2d;

        Rigidbody2DComponent()
            : Component(ComponentName::Rigidbody2D) {}

        Rigidbody2DComponent(const Rigidbody2D& rigidbody2d)
            : Component(ComponentName::Rigidbody2D), Rigidbody2d(rigidbody2d) {}

        Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
    };
}