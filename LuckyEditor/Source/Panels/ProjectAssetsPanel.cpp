#include "ProjectAssetsPanel.h"

#include "Lucky/Core/Log.h"

#include <imgui/imgui.h>

namespace Lucky
{
    static const std::filesystem::path g_AssetPath = "Assets";  // �ʲ���Ŀ¼��

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
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 6)); // ���� Padding���ؼ��߽絽���ڱ߽�ľ��룩
        ImGui::Begin(m_Name.c_str());
        {
            ImGui::PopStyleVar();

            // ������ && �����ͣ�ڸô��ڣ�����հ�λ�ã�
            if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered())
            {
                m_CurrentFile.clear();  // ȡ��ѡ�У��ÿյ�ǰѡ���ļ�·��
            }

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 0));   // �������ڵ�֮��Ĵ�ֱ����Ϊ 0
            
            DrawAssetNode(g_AssetPath); // �����ʲ���
            
            ImGui::PopStyleVar();
        }
        ImGui::End();
    }

    void ProjectAssetsPanel::DrawAssetNode(const std::filesystem::path& fileNode)
    {
        // ��ǰ�ڵ㲻����
        if (!std::filesystem::exists(fileNode))
        {
            return;
        }

        bool isDirectory = std::filesystem::is_directory(fileNode); // ��ǰ�ڵ��Ƿ�ΪĿ¼

        bool isLeaf = true;

        if (isDirectory)
        {
            // �жϵ�ǰ����Ƿ�ΪҶĿ¼��û����Ŀ¼�Ľ�㣩
            for (auto& fileEntry : std::filesystem::directory_iterator(fileNode))
            {
                // �ӽڵ���Ŀ¼�򳣹��ļ�����ǰ�ڵ㲻��Ҷ��㣩
                if (fileEntry.is_directory() || fileEntry.is_regular_file())
                {
                    isLeaf = false;
                    break;
                }
            }
        }

        std::string filename = fileNode.filename().string();    // �ڵ���
        ImGui::PushID(filename.c_str());                        // �ڵ� ID

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_SpanAvailWidth;

        static ImVec4 headerColor = { 0, 0, 0, 0 }; // ���ڵ���ɫ

        // ��ǰ�ڵ㱻ѡ��
        if (m_CurrentFile == fileNode)
        {
            flags |= ImGuiTreeNodeFlags_Selected;

            headerColor = { 0.2588f, 0.5882f, 0.9804f, 0.3490f };   // ����ѡ����ɫ
        }
        else
        {
            headerColor = { 0, 0, 0, 0 };
        }

        ImGuiIO& io = ImGui::GetIO();
        ImFont* font = io.Fonts->Fonts[1];  // Ĭ�����壺1 ������

        // Assets ��Ŀ¼
        if (fileNode == g_AssetPath)
        {
            flags |= ImGuiTreeNodeFlags_DefaultOpen;    // �ʲ���Ŀ¼ Ĭ�ϴ�
            font = io.Fonts->Fonts[0];                  // ���壺0������
        }

        if (isLeaf)
        {
            flags |= ImGuiTreeNodeFlags_Leaf;   // ��Ҷ��� û�м�ͷ
        }

        ImGui::PushStyleColor(ImGuiCol_Header, headerColor);
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, headerColor);
        ImGui::PushStyleColor(ImGuiCol_HeaderActive, headerColor);

        ImVec2 nodePos = ImGui::GetCursorPos(); // Ŀ¼���λ������

        bool opened = ImGui::TreeNodeEx((void*)filename.c_str(), flags, "");    // Ŀ¼���ڵ㣺���id ����־ �������Ŀ¼����
        
        ImGui::PopStyleColor(3);

        // ��ǰ��㱻���ʱ
        if (ImGui::IsItemClicked())
        {
            m_CurrentFile = fileNode;   // �ڵ㱻ѡ��

            LC_TRACE("��ǰѡ���ļ���[{0}]", m_CurrentFile.filename().string());
        }

        uint32_t fileNodeIconID = 0;    // �ļ��ڵ�ͼ�� ID

        // ����Ŀ¼
        if (!isDirectory)
        {
            fileNodeIconID = m_FileIcon->GetRendererID();
        }
        else
        {
            if (opened && !isLeaf)  // �� && ����ҶĿ¼
            {
                fileNodeIconID = m_DirectoryOpenedIcon->GetRendererID();
            }
            else
            {
                fileNodeIconID = m_DirectoryClosedIcon->GetRendererID();
            }
        }

        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(nodePos.x + 32, nodePos.y + 2));                                         // ���� Icon λ��
        ImGui::Image((void*)fileNodeIconID, ImVec2(m_IconSize, m_IconSize), ImVec2(0, 1), ImVec2(1, 0));    // �ļ�ͼ��ͼƬ

        ImGui::SameLine();

        ImGui::SetCursorPos(ImVec2(nodePos.x + 60, nodePos.y)); // ���� Text λ��

        ImGui::PushFont(font);          // ���ý���ı�����
        ImGui::Text(filename.c_str());  // �ļ���
        ImGui::PopFont();

        if (!isDirectory)
        {
            ImGui::TreePop();
            ImGui::PopID();

            return;
        }

        // ��ǰĿ¼����
        if (opened)
        {
            // ������ǰ���
            for (auto& fileEntry : std::filesystem::directory_iterator(fileNode))
            {
                // �ӽ�����
                if (std::filesystem::exists(fileEntry))
                {
                    DrawAssetNode(fileEntry.path());    // ���ƽ��
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