#pragma once

namespace Lucky
{
    /// <summary>
    /// 2D 刚体
    /// </summary>
    class Rigidbody2D
    {
    public:
        /// <summary>
        /// 刚体类型
        /// </summary>
        enum class BodyType
        {
            None = -1,

            Static,     // 静态
            Dynamic,    // 动态
            Kinematic
        };
        /// <summary>
        /// 碰撞检测模式
        /// </summary>
        enum class CollisionDetectionMode
        {
            None = -1,
            Discrete,   // 离散的
            Continuous  // 连续的
        };
    private:
        BodyType m_Type = BodyType::Dynamic;    // 刚体类型

        float m_Mass = 1.0f;            // 质量    [0.0001, 1000000]
        float m_LinearDrag = 0.0f;      // 线性阻尼 [0, 1000000]
        float m_AngularDrag = 0.05f;    // 角阻尼  [0, 1000000]
        float m_GravityScale = 1.0f;    // 重力缩放 [0, 1000000]

        CollisionDetectionMode m_CollisionDetection = CollisionDetectionMode::Discrete; // 碰撞检测 离散 
        
        bool m_FreezeRotation = false;  // 冻结 z 旋转

        void* m_RuntimeBody = nullptr;  // 存储运行时数据
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