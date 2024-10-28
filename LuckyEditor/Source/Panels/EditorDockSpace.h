#pragma once

#include "EditorWindow.h"

namespace Lucky
{
    /// <summary>
    /// �༭��ͣ���ռ䣨��������������ǰ����Ⱦ��
    /// </summary>
    class EditorDockSpace : public EditorWindow
    {
    private:
        // TODO ���� ��ʽ �Ƿ�򿪡�
        // ���� �ر�
        bool m_Opened = true;       // �Ƿ��
        bool m_IsFullScreen = true; // �Ƿ�ȫ��
        uint32_t m_Flags;           // DockSpace ��־��ImGuiDockNodeFlags�� TODO ����
        uint32_t m_WindowFlags;     // DockSpace ���ڱ�־��ImGuiWindowFlags�� TODO ����
    public:
        EditorDockSpace(bool opened = true, bool fullScreen = true);

        virtual ~EditorDockSpace() override;

        virtual void OnImGuiRender() override;
    };
}