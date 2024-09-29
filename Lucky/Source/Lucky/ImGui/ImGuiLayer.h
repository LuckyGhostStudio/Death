#pragma once

#include "Lucky/Core/Layer.h"

#include "Lucky/Events/ApplicationEvent.h"
#include "Lucky/Events/KeyEvent.h"
#include "Lucky/Events/MouseEvent.h"

namespace Lucky
{
    /// <summary>
    /// ImGui 层
    /// </summary>
    class ImGuiLayer : public Layer
    {
    private:
        float m_Time = 0.0f;    // 当前帧时间
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        /// <summary>
        /// 将该层添加到层栈时调用
        /// </summary>
        virtual void OnAttach() override;

        /// <summary>
        /// 将该层从层栈移除时调用
        /// </summary>
        virtual void OnDetach() override;

        void Begin();

        void End();
    };
}