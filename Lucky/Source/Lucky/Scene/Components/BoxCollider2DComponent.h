#pragma once

#include "Component.h"
#include "Lucky/Physics2D/BoxCollider2D.h"

namespace Lucky
{
    /// <summary>
    /// 2D ¸ÕÌå×é¼þ
    /// </summary>
    struct BoxCollider2DComponent : public Component
    {
        BoxCollider2D BoxCollider2d;

        BoxCollider2DComponent()
            : Component(ComponentName::BoxCollider2D) {
        }

        BoxCollider2DComponent(const BoxCollider2D& boxCollider2d)
            : Component(ComponentName::BoxCollider2D), BoxCollider2d(boxCollider2d) {
        }

        BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
    };
}