#pragma once

#include <glm/glm.hpp>

namespace Lucky
{
    /// <summary>
    /// Circle2D ��ײ��
    /// </summary>
    struct CircleCollider2D
    {
        glm::vec2 Offset = { 0.0f, 0.0f };
        float Radius = 0.5f;

        // TODO �ƶ��� Physics Material
        float Density = 1.0f;               // �ܶ�	kg / m ^ 2
        float Friction = 0.5f;              // Ħ����[0, 1]
        float Restitution = 0.0f;           // �ָ�ϵ����Ϊ0��û�е���[0, 1]
        float RestitutionThreshold = 0.5f;  // �ָ���ֵ���ٶȵ��ڴ�ֵ��ֹͣ��������

        void* RuntimeFixture = nullptr;     // �洢����ʱ����

        CircleCollider2D() = default;
        CircleCollider2D(const CircleCollider2D&) = default;
    };
}