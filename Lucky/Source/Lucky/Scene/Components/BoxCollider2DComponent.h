#pragma once

#include "Component.h"
#include "Lucky/Physics2D/BoxCollider2D.h"

namespace Lucky
{
    /// <summary>
    /// 2D ¸ÕÌå×é¼þ
    /// </summary>
    struct BoxCollider2DComponent
    {
        ComponentType Type = ComponentType::BoxCollider2D;

        BoxCollider2D BoxCollider2d;

        BoxCollider2DComponent() = default;
        BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
        BoxCollider2DComponent(const BoxCollider2D& boxCollider2d)
            : BoxCollider2d(boxCollider2d)
        {

        }
    };
}