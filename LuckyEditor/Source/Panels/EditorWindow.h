#pragma once

#include "Lucky/Core/DeltaTime.h"
#include "Lucky/Events/Event.h"

#include <string>

namespace Lucky
{
    /// <summary>
    /// �༭�����ڻ���
    /// </summary>
    class EditorWindow
    {
    protected:
        std::string m_Name;     // ���֣����� ID
    public:
        EditorWindow(const std::string& name = "EditorWindow");
        virtual ~EditorWindow();

        /// <summary>
        /// ���£�ÿ֡����
        /// </summary>
        /// <param name="dt">֡���</param>
        virtual void OnUpdate(DeltaTime dt) = 0;

        /// <summary>
        /// ��Ⱦ ImGui ʱ����
        /// </summary>
        virtual void OnImGuiRender() = 0;

        /// <summary>
        /// �¼�������
        /// </summary>
        /// <param name="event">�¼�</param>
        virtual void OnEvent(Event& event) = 0;

        inline const std::string& GetName() const { return m_Name; }
    };
}