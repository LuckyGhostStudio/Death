#pragma once

namespace Lucky
{
    /// <summary>
    /// �������
    /// </summary>
    enum class ComponentType
    {
        None = -1,

        ID,
        Self,

        Transform,
        SpriteRenderer,
        Camera,

        Rigidbody2D,
        BoxCollider2D,
        CircleCollider2D,

        Script
    };
}