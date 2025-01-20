#pragma once

#include <glm/glm.hpp>

namespace Lucky
{
    /// <summary>
    /// Box2D ��ײ��
    /// </summary>
    struct BoxCollider2D
    {
        glm::vec2 Offset = { 0.0f, 0.0f };
        glm::vec2 Size = { 1.0f, 1.0f };

        // TODO �ƶ��� Physics Material -> Asset
        float Density = 1.0f;               // �ܶ�	kg / m ^ 2
        float Friction = 0.5f;              // Ħ����[0, 1]
        float Restitution = 0.0f;           // �ָ�ϵ����Ϊ0��û�е���[0, 1]
        float RestitutionThreshold = 0.5f;  // �ָ���ֵ���ٶȵ��ڴ�ֵ��ֹͣ��������

        void* RuntimeFixture = nullptr;     // �洢����ʱ����

        BoxCollider2D() = default;
        BoxCollider2D(const BoxCollider2D&) = default;
    };
}