#pragma once

#include "EditorPanel.h"

namespace Lucky
{
    /// <summary>
    /// ��Ⱦ��������壺��ʾ������Ⱦ����
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