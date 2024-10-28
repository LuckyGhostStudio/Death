#pragma once

#include "EditorWindow.h"

#include "Lucky/Core/Core.h"
#include "Lucky/Core/Log.h"

#include "Lucky/Scene/Object.h"

namespace Lucky
{
    /// <summary>
    /// 检视面板：显示选中物体或资产的信息
    /// </summary>
    class InspectorPanel : public EditorWindow
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
        InspectorPanel();
        virtual ~InspectorPanel();

        /// <summary>
        /// 渲染ImGui
        /// </summary>
        void OnImGuiRender();
    };
}