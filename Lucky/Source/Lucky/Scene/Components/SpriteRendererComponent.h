#pragma once

#include "Component.h"
#include "Lucky/Renderer/Texture.h"

#include <glm/glm.hpp>

namespace Lucky
{
    /// <summary>
    /// Í¼Æ¬äÖÈ¾Æ÷×é¼þ
    /// </summary>
    struct SpriteRendererComponent : public Component
    {
        Ref<Texture2D> Sprite;// = CreateRef<Texture2D>(1, 1); // Í¼Æ¬£¨Ä¬ÈÏ°×É«ÎÆÀí£© TODO class Sprite
        glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };          // ÑÕÉ«
        // TODO Flip
        // TODO Material

        SpriteRendererComponent()
            : Component(ComponentName::SpriteRenderer) {}

        SpriteRendererComponent(const glm::vec4& color)
            : Component(ComponentName::SpriteRenderer), Color(color) {}

        SpriteRendererComponent(const SpriteRendererComponent&) = default;
    };
}