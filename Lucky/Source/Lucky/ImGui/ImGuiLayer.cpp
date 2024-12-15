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
    const static float s_StandardDPI = 120.0f;  // ��׼ DPI

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
        ImGui::CreateContext();     // ���� ImGui ������

        Application& app = Application::GetInstance();

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows

        float fontSize = 20.0f * app.GetWindow().GetDPI() / s_StandardDPI;
        io.Fonts->AddFontFromFileTTF("Resources/Fonts/Opensans/OpenSans-Bold.ttf", fontSize);   // ��Ӵ��壨0�ţ�
        
        // Ĭ������ ��� TTF ����
        io.FontDefault = io.Fonts->AddFontFromFileTTF("Resources/Fonts/Opensans/OpenSans-Regular.ttf", fontSize);   // 1��

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            SetDefaultStyles();     // ���ó�����ʽ
            SetDarkThemeColors();   // ������ɫ����
        }

        GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::OnDetach()
    {
        ImGui_ImplOpenGL3_Shutdown();   // �ر� ImGui 
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();        // ����������
    }

    void ImGuiLayer::OnEvent(Event& e)
    {
        // ��ֹ�����¼�
        if (m_BlockEvents)
        {
            ImGuiIO& io = ImGui::GetIO();

            //e.m_Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;        // ��������¼�
            //e.m_Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;  // ��������¼�
        }
    }

    void ImGuiLayer::Begin()
    {
        // ������֡
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
    }

    void ImGuiLayer::End()
    {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::GetInstance();
        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight()); // ���ô��ڴ�С

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
        // TODO ���� ImGuiStyle �����в���
        style.WindowBorderSize = 1.0f;
        style.ChildBorderSize = 1.0f;
        style.TabBorderSize = 1.0f;

        style.WindowMinSize.x = 50.0f;          // ������С�ߴ�

        style.FrameRounding = 4.0f;             // �ؼ��߿�Բ�� [0, 12] 4.8 <=> 0.4f
        style.FrameBorderSize = 1.0f;           // �߿�ߴ�
        style.FramePadding.y = 4.0f;
 
        style.WindowRounding = 4.0f;            // ���ڱ߿�Բ��
        style.GrabRounding = 4.0f;              // �϶��� handle Բ��
        style.PopupRounding = 4.0f;             // ��������Բ��
        style.ChildRounding = 4.0f;             // �Ӵ���Բ��
        style.TabRounding = 2.0f;               // Tab Բ��

        style.ScrollbarRounding = 12.0f;        // ������Բ��
        style.ScrollbarSize = 18.0f;

        style.ButtonTextAlign = { 0.5f, 0.5f }; // ��ť���־���
    }

    void ImGuiLayer::SetDarkThemeColors()
    {
        // TODO ���� Blender �İ�ɫ������ɫ��δ�� -> ���� Unity / UE �İ�ɫ����
        auto& colors = ImGui::GetStyle().Colors;    // ��ɫ��ʽ
        
        //���ڱ���
        colors[ImGuiCol_WindowBg] = ImVec4{ 0.2196f, 0.2196f, 0.2196f, 1.0f };

        // Header
        colors[ImGuiCol_Header] = ImVec4{ 0.2431f, 0.2431f, 0.2431f, 1.0f };
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.2431f, 0.2431f, 0.2431f, 1.0f };

        // ��ť
        colors[ImGuiCol_Button] = ImVec4{ 0.3451f, 0.3451f, 0.3451f, 1.0f };
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3882f, 0.3882f, 0.3882f, 1.0f };
        colors[ImGuiCol_ButtonActive] = ImVec4{ 0.3451f, 0.3451f, 0.3451f, 1.0f };

        // �ؼ�����
        colors[ImGuiCol_FrameBg] = ImVec4{ 0.1569f, 0.1569f, 0.1569f, 1.0f };
        colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_FrameBgActive & ImGuiCol_Border] = ImVec4{ 0.498f, 0.8392f, 0.9921f, 1.0f };

        // ��ǩ TODO ����ʱ�ı���ʽ
        colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabHovered] = colors[ImGuiCol_WindowBg];
        colors[ImGuiCol_TabActive] = colors[ImGuiCol_WindowBg];
        colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive] = colors[ImGuiCol_WindowBg];  // û���۽� ��Ծ״̬

        // ����
        colors[ImGuiCol_TitleBg] = ImVec4{ 0.1569f, 0.1569f, 0.1569f, 1.0f };
        colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // �ؼ��߽���ɫ
        colors[ImGuiCol_Border] = ImVec4{ 0.098f, 0.098f, 0.098f, 1.0f };
        colors[ImGuiCol_BorderShadow] = ImVec4{ 0.051f, 0.051f, 0.051f, 1.0f };

        // �ı���ɫ
        colors[ImGuiCol_Text] = ImVec4{ 0.9f, 0.9f, 0.9f, 1.0f };
        colors[ImGuiCol_TextDisabled] = ImVec4{ 0.5f, 0.5f, 0.5f, 1.0f };   // ����ʱ��ɫ

        // �����˵���ɫ
        colors[ImGuiCol_PopupBg] = ImVec4{ 0.2196f, 0.2196f, 0.2196f, 1.0f };

        // ��ѡ��Ժ���ɫ
        colors[ImGuiCol_CheckMark] = ImVec4{ 0.8863f, 0.8863f, 0.8863f, 1.0f };

        // �˵�������
        colors[ImGuiCol_MenuBarBg] = ImVec4{ 0.094f, 0.094f, 0.094f, 1.0f };

        colors[ImGuiCol_ScrollbarBg] = ImVec4{ 0.2078f, 0.2078f, 0.2078f, 1.0f };
        colors[ImGuiCol_ScrollbarGrab] = ImVec4{ 0.3725f, 0.3725f, 0.3725f, 1.0f };
        colors[ImGuiCol_SliderGrab] = ImVec4{ 0.6f, 0.6f, 0.6f, 1.0f };

        // �ָ���
        colors[ImGuiCol_Separator] = ImVec4{ 0.098f, 0.098f, 0.098f, 1.0f };
        colors[ImGuiCol_SeparatorHovered] = ImVec4{ 0.098f, 0.098f, 0.098f, 1.0f };
        colors[ImGuiCol_SeparatorActive] = ImVec4{ 0.098f, 0.098f, 0.098f, 1.0f };
    }
}