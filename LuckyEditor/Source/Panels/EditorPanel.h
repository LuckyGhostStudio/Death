#pragma once

#include "Lucky/Core/Core.h"
#include "Lucky/Scene/Scene.h"
#include "Lucky/Events/Event.h"

namespace Lucky
{
    /// <summary>
    /// �༭��������
    /// </summary>
    class EditorPanel
    {
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
    };
}
