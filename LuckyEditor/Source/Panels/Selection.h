#pragma once

#include "Lucky/Scene/Object.h"

namespace Lucky
{
    /// <summary>
    /// 选中项
    /// </summary>
    class Selection
    {
    private:
        Object m_Object;
        // TODO 添加资产类型
    public:
        Selection() = default;

        Selection(Object object) : m_Object(object) {}

        void SetSelection(Object object) { m_Object = object; }

        Object GetSelection() const { return m_Object; }
    };
}