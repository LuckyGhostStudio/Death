#pragma once

#include "Lucky/Core/Core.h"
#include "Lucky/Core/Log.h"

#include "Lucky/Scene/Object.h"
#include "Selection.h"

namespace Lucky
{
    /// <summary>
    /// 物体属性面板 TODO 修改为 Inspector
    /// </summary>
    class ObjectPropertiesPanel
    {
    private:
        Selection m_Selection;  // 当前选中项
    private:
        /// <summary>
        /// 绘制物体的所有组件
        /// </summary>
        /// <param name="object">物体</param>
        void DrawComponents(Object object);
    public:
        ObjectPropertiesPanel() = default;

        ObjectPropertiesPanel(Selection selection);

        /// <summary>
        /// 设置选中项
        /// </summary>
        /// <param name="selection">选中项</param>
        void SetSelection(Selection selection);

        /// <summary>
        /// 渲染ImGui
        /// </summary>
        void OnImGuiRender();
    };
}