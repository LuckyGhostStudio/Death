#pragma once

#include "EditorPanel.h"

#include "Lucky/Scene/Object.h"
#include "Lucky/Scene/Selection.h"

#include "Lucky/Renderer/Texture.h"

namespace Lucky
{
    /// <summary>
    /// 检视面板：显示选中物体或资产的信息
    /// </summary>
    class InspectorPanel : public EditorPanel
    {
    private:
        Object& m_SelectionObject = Selection::Object;  // 当前选中项

        Ref<Texture2D> m_SettingsButtonIcon;    // 组件设置按钮 Icon
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
        InspectorPanel() = default;

        virtual void OnImGuiRender(bool& isOpen) override;
    };
}