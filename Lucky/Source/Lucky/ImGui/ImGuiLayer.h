#pragma once

#include "Lucky/Core/Layer.h"

#include "Lucky/Events/ApplicationEvent.h"
#include "Lucky/Events/KeyEvent.h"
#include "Lucky/Events/MouseEvent.h"

namespace Lucky
{
    /// <summary>
    /// ImGui ��
    /// </summary>
    class ImGuiLayer : public Layer
    {
    private:
        float m_Time = 0.0f;    // ��ǰ֡ʱ��
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        /// <summary>
        /// ���ò���ӵ���ջʱ����
        /// </summary>
        virtual void OnAttach() override;

        /// <summary>
        /// ���ò�Ӳ�ջ�Ƴ�ʱ����
        /// </summary>
        virtual void OnDetach() override;

        void Begin();

        void End();
    };
}