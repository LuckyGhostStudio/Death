#pragma once

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
        std::string m_Title;    // ����
    public:
        EditorWindow(const std::string& name = "EditorWindow", const std::string& title = "EditorWindow");
        virtual ~EditorWindow();

        /// <summary>
        /// ��Ⱦ ImGui ʱ����
        /// </summary>
        virtual void OnImGuiRender() = 0;
    };
}