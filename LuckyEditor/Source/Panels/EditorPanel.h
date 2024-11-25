#pragma once

#include "Lucky/Core/Core.h"
#include "Lucky/Scene/Scene.h"
#include "Lucky/Core/Events/Event.h"

namespace Lucky
{
    /// <summary>
    /// �༭��������
    /// </summary>
    class EditorPanel
    {
    protected:
        bool m_IsFocused = false;
    public:
        virtual ~EditorPanel() = default;

        /// <summary>
        /// ��Ⱦ ImGui ʱ����
        /// </summary>
        /// <param name="isOpen">�Ƿ��</param>
        virtual void OnImGuiRender(bool& isOpen) = 0;

        /// <summary>
        /// �¼�������
        /// </summary>
        /// <param name="event">�¼�</param>
        virtual void OnEvent(Event& event) {}

        /// <summary>
        /// ���� Scene ������
        /// </summary>
        /// <param name="context">Scene ������</param>
        virtual void SetSceneContext(const Ref<Scene>& context) {}

        bool IsFocused() const { return m_IsFocused; }
    };
}
