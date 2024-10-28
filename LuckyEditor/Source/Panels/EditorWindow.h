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
    public:
        EditorWindow(const std::string& name = "EditorWindow");
        virtual ~EditorWindow();

        /// <summary>
        /// ��Ⱦ ImGui ʱ����
        /// </summary>
        virtual void OnImGuiRender() = 0;
    };
}