#pragma once

namespace Lucky
{
    typedef unsigned int uint32_t;

    /// <summary>
    /// 编辑器停靠空间
    /// </summary>
    class EditorDockSpace
    {
    private:
        bool m_IsFullScreen = true; // 是否全屏
        uint32_t m_Flags;           // DockSpace 标志（ImGuiDockNodeFlags）
        uint32_t m_WindowFlags;     // DockSpace 窗口标志（ImGuiWindowFlags）
    public:
        EditorDockSpace(bool fullScreen = true);

        void Setup();
    };
}