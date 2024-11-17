#pragma once

#include "EditorPanel.h"

namespace Lucky
{
    /// <summary>
    /// 渲染器数据面板：显示场景渲染数据
    /// </summary>
    class RendererStatsPanel : public EditorPanel
    {
    private:

    public:
        RendererStatsPanel() = default;

        virtual void OnUpdate(DeltaTime dt);

        virtual void OnImGuiRender(bool& isOpen) override;
    };
}