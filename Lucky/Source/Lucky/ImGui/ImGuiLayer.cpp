#include "lcpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

#include "Lucky/Core/Application.h"

// Temp
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ImGuizmo.h"
#include <imgui_internal.h>

namespace Lucky
{
    ImGuiLayer::ImGuiLayer()
        : Layer("ImGuiLayer")
    {
        float dpi = GetDpiForSystem();  // DPI 120 Ϊ��׼ֵ
        LC_TRACE("DPI: {0}", dpi);
    }

    ImGuiLayer::~ImGuiLayer()
    {

    }

    void ImGuiLayer::OnAttach()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();     // ���� ImGui ������

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows

        float fontSize = 20.0f * GetDpiForSystem() / 120;
        io.Fonts->AddFontFromFileTTF("Resources/Fonts/Opensans/OpenSans-Bold.ttf", fontSize);   // ��Ӵ��壨0�ţ�
        
        // Ĭ������ ��� TTF ����
        io.FontDefault = io.Fonts->AddFontFromFileTTF("Resources/Fonts/Opensans/OpenSans-Regular.ttf", fontSize);   // 1��

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            SetDefaultStyles();     // ���ó�����ʽ
            SetDarkThemeColors();   // ������ɫ����
        }

        Application& app = Application::GetInstance();
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

            e.m_Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;        // ��������¼�
            e.m_Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;  // ��������¼�
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
        style.WindowBorderSize = 0;
        style.ChildBorderSize = 0;

        style.WindowMinSize.x = 50.0f;          // ������С�ߴ�

        style.FrameRounding = 4.8f;             // �ؼ��߿�Բ�� [0, 12] 4.8 <=> 0.4f
        style.FrameBorderSize = 1.0f;           // �߿�ߴ�
        style.FramePadding.y = 1.0f;

        style.WindowRounding = 4.8f;            // ���ڱ߿�Բ��
        style.GrabRounding = 4.8f;              // �϶��� handle Բ��
        style.PopupRounding = 4.8f;             // ��������Բ��
        style.ChildRounding = 4.8f;             // �Ӵ���Բ��
        style.TabRounding = 4.8f;               // Tab Բ��

        style.ScrollbarRounding = 12.0f;        // ������Բ��
        style.ScrollbarSize = 20.0f;

        style.ButtonTextAlign = { 0.5f, 0.5f }; // ��ť���־���
    }

    static inline ImVec4 operator*(const ImVec4& lhs, const ImVec4& rhs)
    {
        return ImVec4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
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
        colors[ImGuiCol_BorderShadow] = ImVec4{ 0.1647f, 0.1647f, 0.1647f, 1.0f };

        // �ı���ɫ
        colors[ImGuiCol_Text] = ImVec4{ 0.83f, 0.83f, 0.83f, 1.0f };
        colors[ImGuiCol_TextDisabled] = ImVec4{ 0.5f, 0.5f, 0.5f, 1.0f };   // ����ʱ��ɫ

        // �����˵���ɫ
        colors[ImGuiCol_PopupBg] = ImVec4{ 0.2196f, 0.2196f, 0.2196f, 1.0f };

        // ��ѡ��Ժ���ɫ
        colors[ImGuiCol_CheckMark] = ImVec4{ 0.8863f, 0.8863f, 0.8863f, 1.0f };

        colors[ImGuiCol_MenuBarBg] = ImVec4{ 0.094f, 0.094f, 0.094f, 1.0f };
        colors[ImGuiCol_ScrollbarBg] = ImVec4{ 0.2078f, 0.2078f, 0.2078f, 1.0f };
        colors[ImGuiCol_ScrollbarGrab] = ImVec4{ 0.3725f, 0.3725f, 0.3725f, 1.0f };
        colors[ImGuiCol_SliderGrab] = ImVec4{ 0.6f, 0.6f, 0.6f, 1.0f };

        // �ָ���
        colors[ImGuiCol_Separator] = ImVec4{ 0.098f, 0.098f, 0.098f, 1.0f };
        colors[ImGuiCol_SeparatorHovered] = ImVec4{ 0.098f, 0.098f, 0.098f, 1.0f };
        colors[ImGuiCol_SeparatorActive] = ImVec4{ 0.098f, 0.098f, 0.098f, 1.0f };

        /*
        colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);        // ��ɫ
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);        // ��ɫ
        colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);        // ���ɫ
        colors[ImGuiCol_ChildBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);        // �ϰ��Ļ�ɫ
        colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);        // ���ɫ
        colors[ImGuiCol_Border] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);        // ����ı߿�
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);        // ����Ӱ
        colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);        // ��ɫ�򱳾�
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);        // ���ɫ��ͣ
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);        // ����Ļ�Ծ�򱳾�
        colors[ImGuiCol_TitleBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);        // ���ɫ���ⱳ��
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);        // ���ɫ��Ծ���ⱳ��
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);        // ������۵����ⱳ��
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);        // ���ɫ�˵�������
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.80f);        // ���ɫ����������
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);        // �л�ɫ����
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);        // ��ɫ��ͣ����
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);        // �����Ļ���
        colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);        // ��ɫ��ѡ���
        colors[ImGuiCol_SliderGrab] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);        // ��ɫ����
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);        // ��ɫ��Ծ����
        colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);        // ��͸����ɫ��ť
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);        // ��ɫ��ͣ��ť
        colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);        // ����ɫ��Ծ��ť
        colors[ImGuiCol_Header] = ImVec4(0.20f, 0.20f, 0.20f, 0.80f);        // ���ɫͷ��
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);        // ��ɫ��ͣͷ��
        colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);        // ��ɫ��Ծͷ��
        colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];                    // �߿�ָ���
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);        // ���ɫ��ͣ�ָ���
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);        // ����Ļ�Ծ�ָ���
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.20f, 0.20f, 0.20f, 0.25f);        // �ϵ������ɫ��������
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.30f, 0.30f, 0.30f, 0.70f);        // ���ɫ��ͣ��������
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);        // ����ĵ�������
        colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f); // ��ǩ
        colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];             // ��ǩ��ͣ
        colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f); // ��Ծ��ǩ
        colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f); // ���۽���ǩ
        colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f); // ���۽���Ծ��ǩ
        colors[ImGuiCol_DockingPreview] = colors[ImGuiCol_HeaderActive] * ImVec4(1.0f, 1.0f, 1.0f, 0.7f); // ��ʱͣ��Ԥ��
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);        // ��ͣ������
        colors[ImGuiCol_PlotLines] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);        // ��ɫ������
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);        // ��ͣ������
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);        // ����ɫ��״ͼ
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);        // ��ͣ��״ͼ
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);        // ѡ�б���
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);        // �Ϸ�Ŀ��
        colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);        // ��������
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);        // �������ڸ���
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.70f);        // �������ڱ���
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);        // ģ̬���ڱ���
        */
    }
}