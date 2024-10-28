#pragma once

#include "EditorWindow.h"

namespace Lucky
{
    /// <summary>
    /// 编辑器停靠空间（必须在所有面板的前面渲染）
    /// </summary>
    class EditorDockSpace : public EditorWindow
    {
    private:
        // TODO 名字 样式 是否打开。
        // 开启 关闭
        bool m_Opened = true;       // 是否打开
        bool m_IsFullScreen = true; // 是否全屏
        uint32_t m_Flags;           // DockSpace 标志（ImGuiDockNodeFlags） TODO 整理
        uint32_t m_WindowFlags;     // DockSpace 窗口标志（ImGuiWindowFlags） TODO 整理
    public:
        EditorDockSpace(bool opened = true, bool fullScreen = true);

        virtual ~EditorDockSpace() override;

        virtual void OnImGuiRender() override;
    };
}