#pragma once

#include "EditorWindow.h"

namespace Lucky
{
    /// <summary>
    /// 渲染器数据面板：显示场景渲染数据
    /// </summary>
    class RendererStatsPanel : public EditorWindow
    {
    private:

    public:
        RendererStatsPanel();
        virtual ~RendererStatsPanel() override;

        virtual void OnImGuiRender() override;
    };
}