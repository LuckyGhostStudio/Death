#pragma once

#include <string>

namespace Lucky
{
    /// <summary>
    /// 编辑器窗口基类
    /// </summary>
    class EditorWindow
    {
    protected:
        std::string m_Name;     // 名字：窗口 ID
    public:
        EditorWindow(const std::string& name = "EditorWindow");
        virtual ~EditorWindow();

        /// <summary>
        /// 渲染 ImGui 时调用
        /// </summary>
        virtual void OnImGuiRender() = 0;
    };
}