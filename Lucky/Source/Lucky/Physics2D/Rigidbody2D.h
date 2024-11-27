#pragma once

namespace Lucky
{
    /// <summary>
    /// 2D ����
    /// </summary>
    class Rigidbody2D
    {
    public:
        enum class BodyType
        {
            None = -1,
            Static,
            Dynamic,
            Kinematic
        };
    private:
        BodyType m_Type = BodyType::Dynamic;// ����

        bool m_FreezeRotation = false;      // ���� z ��ת

        void* m_RuntimeBody = nullptr;      // �洢����ʱ����
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