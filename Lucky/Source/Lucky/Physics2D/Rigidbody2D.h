#pragma once

namespace Lucky
{
    /// <summary>
    /// 2D ����
    /// </summary>
    struct Rigidbody2D
    {
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

        BodyType Type = BodyType::Dynamic;  // ��������

        float Mass = 1.0f;              // ����    [0.0001, 1000000]
        float LinearDrag = 0.0f;        // �������� [0, 1000000]
        float AngularDrag = 0.05f;      // ������  [0, 1000000]
        float GravityScale = 1.0f;      // �������� [0, 1000000]

        CollisionDetectionMode CollisionDetection = CollisionDetectionMode::Discrete; // ��ײ��� ��ɢ 
        
        bool FreezeRotation = false;    // ���� z ��ת

        void* RuntimeBody = nullptr;    // �洢����ʱ����

        Rigidbody2D() = default;
        Rigidbody2D(const Rigidbody2D&) = default;
    };
}