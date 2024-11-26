#pragma once

#include "Component.h"
#include "Lucky/Physics2D/Rigidbody2D.h"

namespace Lucky
{
    /// <summary>
    /// 2D ¸ÕÌå×é¼þ
    /// </summary>
    struct Rigidbody2DComponent
    {
        ComponentType Type = ComponentType::Rigidbody2D;

        Rigidbody2D Rigidbody2d;

        Rigidbody2DComponent() = default;
        Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
        Rigidbody2DComponent(const Rigidbody2D& rigidbody2d)
            : Rigidbody2d(rigidbody2d) {}
    };
}