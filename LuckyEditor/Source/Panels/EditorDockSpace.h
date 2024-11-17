#pragma once

namespace Lucky
{
    typedef unsigned int uint32_t;

    /// <summary>
    /// �༭��ͣ���ռ�
    /// </summary>
    class EditorDockSpace
    {
    private:
        bool m_IsFullScreen = true; // �Ƿ�ȫ��
        uint32_t m_Flags;           // DockSpace ��־��ImGuiDockNodeFlags��
        uint32_t m_WindowFlags;     // DockSpace ���ڱ�־��ImGuiWindowFlags��
    public:
        EditorDockSpace(bool fullScreen = true);

        void Setup();
    };
}