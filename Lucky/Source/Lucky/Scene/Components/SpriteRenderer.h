#pragma once

#include <glm/glm.hpp>

namespace Lucky
{
    /// <summary>
    /// Í¼Æ¬äÖÈ¾Æ÷×é¼þ
    /// </summary>
    class SpriteRenderer
    {
    private:
        glm::vec4 m_Color{ 1.0f, 1.0f, 1.0f, 1.0f };    // ÑÕÉ«
    public:
        SpriteRenderer() = default;
        SpriteRenderer(const SpriteRenderer&) = default;
        SpriteRenderer(const glm::vec4& color) : m_Color(color) {}

        glm::vec4& GetColor() { return m_Color; }
    };
}