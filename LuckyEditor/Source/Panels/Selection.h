#pragma once

#include "Lucky/Scene/Object.h"

namespace Lucky
{
    /// <summary>
    /// ѡ����
    /// </summary>
    class Selection
    {
    private:
        Object m_Object;
        // TODO ����ʲ�����
    public:
        Selection() = default;

        Selection(Object object) : m_Object(object) {}

        void SetSelection(Object object) { m_Object = object; }

        Object GetSelection() const { return m_Object; }
    };
}