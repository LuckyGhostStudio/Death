#pragma once

#include "EditorWindow.h"

namespace Lucky
{
    /// <summary>
    /// ��Ⱦ��������壺��ʾ������Ⱦ����
    /// </summary>
    class RendererStatsPanel : public EditorWindow
    {
    private:

    public:
        RendererStatsPanel();
        virtual ~RendererStatsPanel() override;

        virtual void OnUpdate(DeltaTime dt) override;

        virtual void OnImGuiRender() override;

        virtual void OnEvent(Event& e) override;
    };
}