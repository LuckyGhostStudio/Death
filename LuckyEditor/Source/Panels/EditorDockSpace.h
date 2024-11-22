#pragma once

#include <functional>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

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

        /// <summary>
        /// ���� DockSpace
        /// </summary>
        /// <typeparam name="UIFunc">UI ��������</typeparam>
        /// <param name="uiToolBarFunc">ToolBar ���ƺ���</param>
        template<typename UIFunc>
        void Setup(UIFunc uiToolBarFunc);
    };

    template<typename UIFunc>
    inline void EditorDockSpace::Setup(UIFunc uiToolBarFunc)
    {
        if (m_IsFullScreen)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();

            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);

            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

            m_WindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            m_WindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        if (m_Flags)
        {
            m_WindowFlags |= ImGuiWindowFlags_NoBackground;
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        //ImGui::PushStyleColor(ImGuiCol_WindowBg, ImGui::GetStyle().Colors[ImGuiCol_MenuBarBg]);
        // DockSpace ����
        ImGui::Begin("DockSpace", nullptr, m_WindowFlags);
        {
            //ImGui::PopStyleColor();
            ImGui::PopStyleVar();

            if (m_IsFullScreen)
            {
                ImGui::PopStyleVar(2);
            }

            ImGuiIO& io = ImGui::GetIO();

            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                uiToolBarFunc();    // ��Ⱦ ToolBar
                ImGui::DockSpace(ImGui::GetID("EditorDockSpace"), ImVec2(0.0f, 0.0f), m_Flags | ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton);
            }
        }
        ImGui::End();
    }
}