#include "ProjectAssetsPanel.h"

#include "Lucky/Core/Log.h"

#include <imgui/imgui.h>

namespace Lucky
{
    static const std::filesystem::path g_AssetPath = "Assets";  // 资产根目录名

    ProjectAssetsPanel::ProjectAssetsPanel()
        : EditorWindow("Project"),
        m_CurrentFile("")
    {
        m_DirectoryClosedIcon = Texture2D::Create("Resources/Icons/DirectoryClosed_Icon.png");
        m_DirectoryOpenedIcon = Texture2D::Create("Resources/Icons/DirectoryOpened_Icon.png");
        m_FileIcon = Texture2D::Create("Resources/Icons/File_Icon.png");
    }

    ProjectAssetsPanel::~ProjectAssetsPanel()
    {

    }

    void ProjectAssetsPanel::OnUpdate(DeltaTime dt)
    {

    }

    void ProjectAssetsPanel::OnImGuiRender()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 6)); // 窗口 Padding（控件边界到窗口边界的距离）
        ImGui::Begin(m_Name.c_str());
        {
            ImGui::PopStyleVar();

            // 点击鼠标 && 鼠标悬停在该窗口（点击空白位置）
            if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered())
            {
                m_CurrentFile.clear();  // 取消选中：置空当前选中文件路径
            }

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 0));   // 设置树节点之间的垂直距离为 0
            
            DrawAssetNode(g_AssetPath); // 绘制资产树
            
            ImGui::PopStyleVar();
        }
        ImGui::End();
    }

    void ProjectAssetsPanel::DrawAssetNode(const std::filesystem::path& fileNode)
    {
        // 当前节点不存在
        if (!std::filesystem::exists(fileNode))
        {
            return;
        }

        bool isDirectory = std::filesystem::is_directory(fileNode); // 当前节点是否为目录

        bool isLeaf = true;

        if (isDirectory)
        {
            // 判断当前结点是否为叶目录（没有子目录的结点）
            for (auto& fileEntry : std::filesystem::directory_iterator(fileNode))
            {
                // 子节点是目录或常规文件（当前节点不是叶结点）
                if (fileEntry.is_directory() || fileEntry.is_regular_file())
                {
                    isLeaf = false;
                    break;
                }
            }
        }

        std::string filename = fileNode.filename().string();    // 节点名
        ImGui::PushID(filename.c_str());                        // 节点 ID

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_SpanAvailWidth;

        static ImVec4 headerColor = { 0, 0, 0, 0 }; // 树节点颜色

        // 当前节点被选中
        if (m_CurrentFile == fileNode)
        {
            flags |= ImGuiTreeNodeFlags_Selected;

            headerColor = { 0.2588f, 0.5882f, 0.9804f, 0.3490f };   // 设置选中颜色
        }
        else
        {
            headerColor = { 0, 0, 0, 0 };
        }

        ImGuiIO& io = ImGui::GetIO();
        ImFont* font = io.Fonts->Fonts[1];  // 默认字体：1 号字体

        // Assets 根目录
        if (fileNode == g_AssetPath)
        {
            flags |= ImGuiTreeNodeFlags_DefaultOpen;    // 资产根目录 默认打开
            font = io.Fonts->Fonts[0];                  // 粗体：0号字体
        }

        if (isLeaf)
        {
            flags |= ImGuiTreeNodeFlags_Leaf;   // 是叶结点 没有箭头
        }

        ImGui::PushStyleColor(ImGuiCol_Header, headerColor);
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, headerColor);
        ImGui::PushStyleColor(ImGuiCol_HeaderActive, headerColor);

        ImVec2 nodePos = ImGui::GetCursorPos(); // 目录结点位置坐标

        bool opened = ImGui::TreeNodeEx((void*)filename.c_str(), flags, "");    // 目录树节点：结点id 结点标志 结点名（目录名）
        
        ImGui::PopStyleColor(3);

        // 当前结点被点击时
        if (ImGui::IsItemClicked())
        {
            m_CurrentFile = fileNode;   // 节点被选中

            LC_TRACE("当前选中文件：[{0}]", m_CurrentFile.filename().string());
        }

        uint32_t fileNodeIconID = 0;    // 文件节点图标 ID

        // 不是目录
        if (!isDirectory)
        {
            fileNodeIconID = m_FileIcon->GetRendererID();
        }
        else
        {
            if (opened && !isLeaf)  // 打开 && 不是叶目录
            {
                fileNodeIconID = m_DirectoryOpenedIcon->GetRendererID();
            }
            else
            {
                fileNodeIconID = m_DirectoryClosedIcon->GetRendererID();
            }
        }

        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(nodePos.x + 32, nodePos.y + 2));                                         // 设置 Icon 位置
        ImGui::Image((void*)fileNodeIconID, ImVec2(m_IconSize, m_IconSize), ImVec2(0, 1), ImVec2(1, 0));    // 文件图标图片

        ImGui::SameLine();

        ImGui::SetCursorPos(ImVec2(nodePos.x + 60, nodePos.y)); // 设置 Text 位置

        ImGui::PushFont(font);          // 设置结点文本字体
        ImGui::Text(filename.c_str());  // 文件名
        ImGui::PopFont();

        if (!isDirectory)
        {
            ImGui::TreePop();
            ImGui::PopID();

            return;
        }

        // 当前目录结点打开
        if (opened)
        {
            // 迭代当前结点
            for (auto& fileEntry : std::filesystem::directory_iterator(fileNode))
            {
                // 子结点存在
                if (std::filesystem::exists(fileEntry))
                {
                    DrawAssetNode(fileEntry.path());    // 绘制结点
                }
            }
            ImGui::TreePop();
        }

        ImGui::PopID();
    }

    void ProjectAssetsPanel::OnEvent(Event& e)
    {

    }
}