#pragma once

#include <glm/glm.hpp>

namespace Lucky
{
    /// <summary>
    /// Box2D ��ײ��
    /// </summary>
    class BoxCollider2D
    {
    private:
        glm::vec2 m_Offset = { 0.0f, 0.0f };
        glm::vec2 m_Size = { 1.0f, 1.0f };

        // TODO �ƶ��� Physics Material
        float m_Density = 1.0f;                 // �ܶ�	kg / m ^ 2
        float m_Friction = 0.5f;                // Ħ����[0, 1]
        float m_Restitution = 0.0f;             // �ָ�ϵ����Ϊ0��û�е���[0, 1]
        float m_RestitutionThreshold = 0.5f;    // �ָ���ֵ���ٶȵ��ڴ�ֵ��ֹͣ��������

        void* m_RuntimeFixture = nullptr;       // �洢����ʱ����
    public:
        BoxCollider2D() = default;
        BoxCollider2D(const BoxCollider2D&) = default;

        // TODO ���ò���ʱ���� fixture ������ ((b2FixtureDef*)bc2d.GetRuntimeFixture())->density = ...
        glm::vec2& GetOffset() { return m_Offset; }
        void SetOffset(const glm::vec2& offset) { m_Offset = offset; }

        glm::vec2& GetSize() { return m_Size; }
        void SetSize(const glm::vec2& size) { m_Size = size; }

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