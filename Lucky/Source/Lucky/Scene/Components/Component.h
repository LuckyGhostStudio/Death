#pragma once

namespace Lucky
{
    /// <summary>
    /// 组件类型
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