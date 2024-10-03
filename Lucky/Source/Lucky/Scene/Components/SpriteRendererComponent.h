#pragma once

#include "Component.h"

#include <glm/glm.hpp>

namespace Lucky
{
    /// <summary>
    /// 图片渲染器组件
    /// </summary>
    struct SpriteRendererComponent : public Component
    {
        glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };  // 颜色
        // TODO: 添加 Sprite

        SpriteRendererComponent()
            : Component(ComponentName::SpriteRenderer) {}

        SpriteRendererComponent(const glm::vec4& color)
            : Component(ComponentName::SpriteRenderer), Color(color) {}

        SpriteRendererComponent(const SpriteRendererComponent&) = default;
    };
}