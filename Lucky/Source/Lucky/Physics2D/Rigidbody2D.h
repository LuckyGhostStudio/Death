#pragma once

namespace Lucky
{
    /// <summary>
    /// 2D 刚体
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
        BodyType m_Type = BodyType::Dynamic;// 类型

        bool m_FreezeRotation = false;      // 冻结 z 旋转

        void* m_RuntimeBody = nullptr;      // 存储运行时数据
    public:
        Rigidbody2D() = default;
        Rigidbody2D(const Rigidbody2D&) = default;

        // TODO 设置参数时更新 b2Body 数据
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