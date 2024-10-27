#pragma once

namespace Lucky
{
    /// <summary>
    /// ͣ���ռ�
    /// </summary>
    class DockSpace
    {
    private:
        // TODO ���� ��ʽ �Ƿ�򿪡�
        // ���� �ر�
        bool m_Opened = true;       // �Ƿ��
        bool m_IsFullScreen = true; // �Ƿ�ȫ��
        uint32_t m_Flags;           // DockSpace ��־��ImGuiDockNodeFlags�� TODO ����
        uint32_t m_WindowFlags;     // DockSpace ���ڱ�־��ImGuiWindowFlags�� TODO ����
    public:

        DockSpace(bool opened = true, bool fullScreen = true);

        ~DockSpace() = default;

        void OnImGuiRender();
    };
}