#pragma once

namespace Lucky
{
    /// <summary>
    /// �������
    /// </summary>
    enum class ComponentName
    {
        None = 0,

        Self,

        Transform,
        SpriteRenderer,
    };

    /// <summary>
    /// �������
    /// </summary>
    struct Component
    {
        ComponentName Name; // �����
        bool Enable;        // ����״̬
        bool IsDefault;     // �Ƿ�ΪĬ�����

        Component() = default;
        Component(const Component&) = default;

        /// <summary>
        /// ���
        /// </summary>
        /// <param name="name">�����</param>
        /// <param name="enable">����״̬</param>
        /// <param name="isDefault">�Ƿ�ΪĬ�����</param>
        Component(ComponentName name = ComponentName::None, bool isDefault = false)
            : Name(name), Enable(true), IsDefault(isDefault) {}

        virtual ~Component() = default;
    };
}