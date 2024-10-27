#pragma once

namespace Lucky
{
    /// <summary>
    /// 停靠空间
    /// </summary>
    class DockSpace
    {
    private:
        // TODO 名字 样式 是否打开。
        // 开启 关闭
        bool m_Opened = true;       // 是否打开
        bool m_IsFullScreen = true; // 是否全屏
        uint32_t m_Flags;           // DockSpace 标志（ImGuiDockNodeFlags） TODO 整理
        uint32_t m_WindowFlags;     // DockSpace 窗口标志（ImGuiWindowFlags） TODO 整理
    public:

        DockSpace(bool opened = true, bool fullScreen = true);

        ~DockSpace() = default;

        void OnImGuiRender();
    };
}