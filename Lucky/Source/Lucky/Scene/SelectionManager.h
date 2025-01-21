#pragma once

#include "Lucky/Scene/Object.h"

namespace Lucky
{
    /// <summary>
    /// 选中项管理器
    /// </summary>
    struct SelectionManager
    {
    private:
        inline static Object s_Context;
    public:
        static void Select(Object selectionID);
        static bool IsSelected(Object selectionID);
        static void Deselect();
        static Object GetSelection();
    };
}