#pragma once

namespace Lucky
{
    /// <summary>
    /// 2D ����
    /// </summary>
    class Rigidbody2D
    {
    public:
        /// <summary>
        /// ��������
        /// </summary>
        enum class BodyType
        {
            None = -1,

            Static,     // ��̬
            Dynamic,    // ��̬
            Kinematic
        };
        /// <summary>
        /// ��ײ���ģʽ
        /// </summary>
        enum class CollisionDetectionMode
        {
            None = -1,
            Discrete,   // ��ɢ��
            Continuous  // ������
        };
    private:
        BodyType m_Type = BodyType::Dynamic;    // ��������

        float m_Mass = 1.0f;            // ����    [0.0001, 1000000]
        float m_LinearDrag = 0.0f;      // �������� [0, 1000000]
        float m_AngularDrag = 0.05f;    // ������  [0, 1000000]
        float m_GravityScale = 1.0f;    // �������� [0, 1000000]

        CollisionDetectionMode m_CollisionDetection = CollisionDetectionMode::Discrete; // ��ײ��� ��ɢ 
        
        bool m_FreezeRotation = false;  // ���� z ��ת

        void* m_RuntimeBody = nullptr;  // �洢����ʱ����
    public:
        Rigidbody2D() = default;
        Rigidbody2D(const Rigidbody2D&) = default;

        // TODO ���ò���ʱ���� b2Body ����
        BodyType GetBodyType() const { return m_Type; }
        void SetBodyType(BodyType type)
        {
            m_Type = type;

            // TODO ((b2Body*)rb2d.GetRuntimeBody())->SetType(Rigidbody2DTypeToBox2DBody(m_Type))
        }

        float GetMass() const { return m_Mass; }
        float& GetMass_Ref() { return m_Mass; }
        void SetMass(float mass) { m_Mass = mass; }

        float GetLinearDrag() const { return m_LinearDrag; }
        float& GetLinearDrag_Ref() { return m_LinearDrag; }
        void SetLinearDrag(float drag) { m_LinearDrag = drag; }

        float GetAngularDrag() const { return m_AngularDrag; }
        float& GetAngularDrag_Ref() { return m_AngularDrag; }
        void SetAngularDrag(float drag) { m_AngularDrag = drag; }

        float GetGravityScale() const { return m_GravityScale; }
        float& GetGravityScale_Ref() { return m_GravityScale; }
        void SetGravityScale(float gravityScale) { m_GravityScale = gravityScale; }

        CollisionDetectionMode GetCollisionDetectionMode() const { return m_CollisionDetection; }
        void SetCollisionDetectionMode(CollisionDetectionMode mode) { m_CollisionDetection = mode; }

        bool IsFreezeRotation() const { return m_FreezeRotation; }
        bool& IsFreezeRotation_Ref()
        {
            // TODO ((b2Body*)rb2d.GetRuntimeBody())->SetFixedRotation()
            return m_FreezeRotation;
        }

        void FreezeRotation(bool freeze)
        {
            m_FreezeRotation = freeze;
            // TODO ((b2Body*)rb2d.GetRuntimeBody())->SetFixedRotation()
        }

        void* GetRuntimeBody() { return m_RuntimeBody; }
        void SetRuntimeBody(void* body) { m_RuntimeBody = body; }
    };
}