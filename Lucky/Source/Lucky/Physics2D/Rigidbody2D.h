#pragma once

namespace Lucky
{
    /// <summary>
    /// 2D 刚体
    /// </summary>
    struct Rigidbody2D
    {
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

        BodyType Type = BodyType::Dynamic;  // 刚体类型

        float Mass = 1.0f;              // 质量    [0.0001, 1000000]
        float LinearDrag = 0.0f;        // 线性阻尼 [0, 1000000]
        float AngularDrag = 0.05f;      // 角阻尼  [0, 1000000]
        float GravityScale = 1.0f;      // 重力缩放 [0, 1000000]

        CollisionDetectionMode CollisionDetection = CollisionDetectionMode::Discrete; // 碰撞检测 离散 
        
        bool FreezeRotation = false;    // 冻结 z 旋转

        void* RuntimeBody = nullptr;    // 存储运行时数据

        Rigidbody2D() = default;
        Rigidbody2D(const Rigidbody2D&) = default;
    };
}