#include "lcpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Lucky/Core/Application.h"

// Temp
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ImGuizmo.h"
#include <imgui_internal.h>

namespace Lucky
{
    const static float s_StandardDPI = 120.0f;  // 基准 DPI

    ImGuiLayer::ImGuiLayer()
        : Layer("ImGuiLayer")
    {

    }

    ImGuiLayer::~ImGuiLayer()
    {

    }

    void ImGuiLayer::OnAttach()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();     // 创建 ImGui 上下文

        Application& app = Application::GetInstance();

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows

        float fontSize = 20.0f * app.GetWindow().GetDPI() / s_StandardDPI;
        io.Fonts->AddFontFromFileTTF("Resources/Fonts/Opensans/OpenSans-Bold.ttf", fontSize);   // 添加粗体（0号）
        
        // 默认字体 添加 TTF 字体
        io.FontDefault = io.Fonts->AddFontFromFileTTF("Resources/Fonts/Opensans/OpenSans-Regular.ttf", fontSize);   // 1号

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            SetDefaultStyles();     // 设置常规样式
            SetDarkThemeColors();   // 设置深色主题
        }

        GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::OnDetach()
    {
        ImGui_ImplOpenGL3_Shutdown();   // 关闭 ImGui 
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();        // 销毁上下文
    }

    void ImGuiLayer::OnEvent(Event& e)
    {
        // 阻止接收事件
        if (m_BlockEvents)
        {
            ImGuiIO& io = ImGui::GetIO();

            //e.m_Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;        // 捕获鼠标事件
            //e.m_Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;  // 捕获键盘事件
        }
    }

    void ImGuiLayer::Begin()
    {
        // 开启新帧
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
    }

    void ImGuiLayer::End()
    {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::GetInstance();
        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight()); // 设置窗口大小

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void ImGuiLayer::SetDefaultStyles()
    {
        ImGuiStyle& style = ImGui::GetStyle();
        // TODO 设置 ImGuiStyle 的所有参数
        style.WindowBorderSize = 1.0f;
        style.ChildBorderSize = 1.0f;
        style.TabBorderSize = 1.0f;

        style.WindowMinSize.x = 50.0f;          // 窗口最小尺寸

        style.FrameRounding = 4.0f;             // 控件边框圆度 [0, 12] 4.8 <=> 0.4f
        style.FrameBorderSize = 1.0f;           // 边框尺寸
        style.FramePadding.y = 4.0f;
 
        style.WindowRounding = 4.0f;            // 窗口边框圆度
        style.GrabRounding = 4.0f;              // 拖动条 handle 圆度
        style.PopupRounding = 4.0f;             // 弹出窗口圆度
        style.ChildRounding = 4.0f;             // 子窗口圆度
        style.TabRounding = 2.0f;               // Tab 圆度

        style.ScrollbarRounding = 12.0f;        // 滚动条圆度
        style.ScrollbarSize = 18.0f;

        style.ButtonTextAlign = { 0.5f, 0.5f }; // 按钮文字居中
    }

    void ImGuiLayer::SetDarkThemeColors()
    {
        // TODO 参照 Blender 的暗色主题配色：未来 -> 设置 Unity / UE 的暗色主题
        auto& colors = ImGui::GetStyle().Colors;    // 颜色样式
        
        //窗口背景
        colors[ImGuiCol_WindowBg] = ImVec4{ 0.2196f, 0.2196f, 0.2196f, 1.0f };

        // Header
        colors[ImGuiCol_Header] = ImVec4{ 0.2431f, 0.2431f, 0.2431f, 1.0f };
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.2431f, 0.2431f, 0.2431f, 1.0f };

        // 按钮
        colors[ImGuiCol_Button] = ImVec4{ 0.3451f, 0.3451f, 0.3451f, 1.0f };
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3882f, 0.3882f, 0.3882f, 1.0f };
        colors[ImGuiCol_ButtonActive] = ImVec4{ 0.3451f, 0.3451f, 0.3451f, 1.0f };

        // 控件背景
        colors[ImGuiCol_FrameBg] = ImVec4{ 0.1569f, 0.1569f, 0.1569f, 1.0f };
        colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_FrameBgActive & ImGuiCol_Border] = ImVec4{ 0.498f, 0.8392f, 0.9921f, 1.0f };

        // 标签 TODO 激活时改变样式
        colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabHovered] = colors[ImGuiCol_WindowBg];
        colors[ImGuiCol_TabActive] = colors[ImGuiCol_WindowBg];
        colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive] = colors[ImGuiCol_WindowBg];  // 没被聚焦 活跃状态

        // 标题
        colors[ImGuiCol_TitleBg] = ImVec4{ 0.1569f, 0.1569f, 0.1569f, 1.0f };
        colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // 控件边界颜色
        colors[ImGuiCol_Border] = ImVec4{ 0.098f, 0.098f, 0.098f, 1.0f };
        colors[ImGuiCol_BorderShadow] = ImVec4{ 0.051f, 0.051f, 0.051f, 1.0f };

        // 文本颜色
        colors[ImGuiCol_Text] = ImVec4{ 0.9f, 0.9f, 0.9f, 1.0f };
        colors[ImGuiCol_TextDisabled] = ImVec4{ 0.5f, 0.5f, 0.5f, 1.0f };   // 禁用时颜色

        // 弹出菜单颜色
        colors[ImGuiCol_PopupBg] = ImVec4{ 0.2196f, 0.2196f, 0.2196f, 1.0f };

        // 勾选框对号颜色
        colors[ImGuiCol_CheckMark] = ImVec4{ 0.8863f, 0.8863f, 0.8863f, 1.0f };

        // 菜单条背景
        colors[ImGuiCol_MenuBarBg] = ImVec4{ 0.094f, 0.094f, 0.094f, 1.0f };

        colors[ImGuiCol_ScrollbarBg] = ImVec4{ 0.2078f, 0.2078f, 0.2078f, 1.0f };
        colors[ImGuiCol_ScrollbarGrab] = ImVec4{ 0.3725f, 0.3725f, 0.3725f, 1.0f };
        colors[ImGuiCol_SliderGrab] = ImVec4{ 0.6f, 0.6f, 0.6f, 1.0f };

        // 分隔线
        colors[ImGuiCol_Separator] = ImVec4{ 0.098f, 0.098f, 0.098f, 1.0f };
        colors[ImGuiCol_SeparatorHovered] = ImVec4{ 0.098f, 0.098f, 0.098f, 1.0f };
        colors[ImGuiCol_SeparatorActive] = ImVec4{ 0.098f, 0.098f, 0.098f, 1.0f };
    }
}