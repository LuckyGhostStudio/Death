#pragma once

#include <functional>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

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

        /// <summary>
        /// 设置 DockSpace
        /// </summary>
        /// <typeparam name="UIFunc">UI 函数类型</typeparam>
        /// <param name="uiToolBarFunc">ToolBar 绘制函数</param>
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

            m_WindowFlags |= ImGuiWindowFlags_NoTitleBar;
            m_WindowFlags |= ImGuiWindowFlags_NoCollapse;
            m_WindowFlags |= ImGuiWindowFlags_NoResize;
            m_WindowFlags |= ImGuiWindowFlags_NoMove;;
            m_WindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
            m_WindowFlags |= ImGuiWindowFlags_NoNavFocus;
        }

        if (m_Flags)
        {
            m_WindowFlags |= ImGuiWindowFlags_NoBackground;
        }

        static ImVec4 barColor = ImGui::GetStyle().Colors[ImGuiCol_MenuBarBg];
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, barColor);
        ImGui::PushStyleColor(ImGuiCol_Border, barColor);
        // DockSpace 窗口
        ImGui::Begin("DockSpace", nullptr, m_WindowFlags);
        {
            ImGui::PopStyleColor(2);
            ImGui::PopStyleVar();

            if (m_IsFullScreen)
            {
                ImGui::PopStyleVar(2);
            }

            ImGuiIO& io = ImGui::GetIO();

            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                uiToolBarFunc();    // 渲染 ToolBar

                ImGui::DockSpace(ImGui::GetID("EditorDockSpace"), ImVec2(0.0f, 0.0f), m_Flags | ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton);
            }
        }
        ImGui::End();
    }
}