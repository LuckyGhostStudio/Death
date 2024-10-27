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
        std::string m_Title;    // 标题
    public:
        EditorWindow(const std::string& name = "EditorWindow", const std::string& title = "EditorWindow");
        virtual ~EditorWindow();

        /// <summary>
        /// 渲染 ImGui 时调用
        /// </summary>
        virtual void OnImGuiRender() = 0;
    };
}