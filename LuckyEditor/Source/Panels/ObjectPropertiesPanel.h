#pragma once

#include "Lucky/Core/Core.h"
#include "Lucky/Core/Log.h"

#include "Lucky/Scene/Object.h"

namespace Lucky
{
    /// <summary>
    /// 物体属性面板 TODO 修改为 Inspector
    /// </summary>
    class ObjectPropertiesPanel
    {
    private:
        /// <summary>
        /// 添加组件 UI
        /// </summary>
        /// <param name="object"></param>
        void AddComponents(Object object);

        /// <summary>
        /// 绘制物体的所有组件 UI
        /// </summary>
        /// <param name="object">物体</param>
        void DrawComponents(Object object);
    public:
        ObjectPropertiesPanel() = default;

        /// <summary>
        /// 渲染ImGui
        /// </summary>
        void OnImGuiRender();
    };
}