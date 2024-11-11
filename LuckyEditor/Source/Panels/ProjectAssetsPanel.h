#pragma once

#include "EditorWindow.h"
#include "Lucky/Renderer/Texture.h"

#include <filesystem>

namespace Lucky
{
    /// <summary>
    /// ��Ŀ�ʲ����
    /// </summary>
    class ProjectAssetsPanel : public EditorWindow
    {
    private:
        std::filesystem::path m_CurrentFile;    // ��ǰ�ļ��ڵ�

        Ref<Texture2D> m_DirectoryClosedIcon;   // Ŀ¼�ر�ͼ��
        Ref<Texture2D> m_DirectoryOpenedIcon;   // Ŀ¼��ͼ��
        Ref<Texture2D> m_FileIcon;              // �ļ�ͼ��
    private:
        /// <summary>
        /// �����ʲ��ڵ�
        /// </summary>
        /// <param name="node">�ʲ��ļ��ڵ�</param>
        void DrawAssetNode(const std::filesystem::path& fileNode);
    public:
        ProjectAssetsPanel();
        virtual ~ProjectAssetsPanel() override;

        virtual void OnUpdate(DeltaTime dt) override;

        virtual void OnImGuiRender() override;

        virtual void OnEvent(Event& e) override;
    };
}