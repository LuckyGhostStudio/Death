#pragma once

#include "Component.h"

#include <glm/glm.hpp>

namespace Lucky
{
    /// <summary>
    /// ͼƬ��Ⱦ�����
    /// </summary>
    struct SpriteRendererComponent : public Component
    {
        glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };  // ��ɫ
        // TODO: ��� Sprite

        SpriteRendererComponent()
            : Component(ComponentName::SpriteRenderer) {}

        SpriteRendererComponent(const glm::vec4& color)
            : Component(ComponentName::SpriteRenderer), Color(color) {}

        SpriteRendererComponent(const SpriteRendererComponent&) = default;
    };
}