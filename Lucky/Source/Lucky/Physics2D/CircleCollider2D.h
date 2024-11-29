#pragma once

#include <glm/glm.hpp>

namespace Lucky
{
    /// <summary>
    /// Circle2D ��ײ��
    /// </summary>
    class CircleCollider2D
    {
    private:
        glm::vec2 m_Offset = { 0.0f, 0.0f };
        float m_Radius = 0.5f;

        // TODO �ƶ��� Physics Material
        float m_Density = 1.0f;                 // �ܶ�	kg / m ^ 2
        float m_Friction = 0.5f;                // Ħ����[0, 1]
        float m_Restitution = 0.0f;             // �ָ�ϵ����Ϊ0��û�е���[0, 1]
        float m_RestitutionThreshold = 0.5f;    // �ָ���ֵ���ٶȵ��ڴ�ֵ��ֹͣ��������

        void* m_RuntimeFixture = nullptr;       // �洢����ʱ����
    public:
        CircleCollider2D() = default;
        CircleCollider2D(const CircleCollider2D&) = default;

        // TODO ���ò���ʱ���� fixture ������ ((b2FixtureDef*)bc2d.GetRuntimeFixture())->density = ...
        glm::vec2& GetOffset() { return m_Offset; }
        void SetOffset(const glm::vec2& offset) { m_Offset = offset; }

        float GetRadius() const { return m_Radius; }
        float& GetRadius_Ref() { return m_Radius; }
        void SetRadius(float radius) { m_Radius = radius; }

        float GetDensity() const { return m_Density; }
        float& GetDensity_Ref() { return m_Density; }
        void SetDensity(bool density) { m_Density = density; }

        float GetFriction() const { return m_Friction; }
        float& GetFriction_Ref() { return m_Friction; }
        void SetFriction(float friction) { m_Friction = friction; }

        float GetRestitution() const { return m_Restitution; }
        float& GetRestitution_Ref() { return m_Restitution; }
        void SetRestitution(float restitution) { m_Restitution = restitution; }

        float GetRestitutionThreshold() const { return m_RestitutionThreshold; }
        void SetRestitutionThreshold(float threshold) { m_RestitutionThreshold = threshold; }

        void* GetRuntimeFixture() const { return m_RuntimeFixture; }
        void SetRuntimeFixture(void* fixture) { m_RuntimeFixture = fixture; }
    };
}