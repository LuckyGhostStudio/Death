#pragma once

#include "Component.h"
#include "Lucky/Renderer/Texture.h"

#include <glm/glm.hpp>

namespace Lucky
{
    /// <summary>
    /// ͼƬ��Ⱦ�����
    /// </summary>
    struct SpriteRendererComponent
    {
        ComponentType Type = ComponentType::SpriteRenderer;

        Ref<Texture2D> Sprite;// = CreateRef<Texture2D>(1, 1); // ͼƬ��Ĭ�ϰ�ɫ���� TODO class Sprite
        glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };          // ��ɫ
        // TODO Flip
        // TODO Material

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& color)
            : Color(color) {}
    };
}