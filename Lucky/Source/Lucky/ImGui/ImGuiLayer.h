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
        float m_Time = 0.0f;        // ��ǰ֡ʱ��
        bool m_BlockEvents = true;  // �Ƿ���ֹ�����¼�
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

        /// <summary>
        /// �¼��ص�����
        /// </summary>
        /// <param name="e">�¼�</param>
        virtual void OnEvent(Event& e) override;

        void Begin();

        void End();

        /// <summary>
        /// ��ֹ�¼�����ֹ�����¼�
        /// </summary>
        /// <param name="block">�Ƿ���ֹ</param>
        void BlockEvents(bool block) { m_BlockEvents = block; }

        /// <summary>
        /// ���ó�����ʽ��������ʽ��
        /// </summary>
        void SetDefaultStyles();

        /// <summary>
        /// ������ɫ���⣺������ɫ��ʽ
        /// </summary>
        void SetDarkThemeColors();
    };
}