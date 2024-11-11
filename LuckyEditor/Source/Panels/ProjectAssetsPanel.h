#pragma once

#include "EditorWindow.h"
#include "Lucky/Renderer/Texture.h"

#include <filesystem>

namespace Lucky
{
    /// <summary>
    /// 项目资产面板
    /// </summary>
    class ProjectAssetsPanel : public EditorWindow
    {
    private:
        std::filesystem::path m_CurrentFile;    // 当前文件节点

        Ref<Texture2D> m_DirectoryClosedIcon;   // 目录关闭图标
        Ref<Texture2D> m_DirectoryOpenedIcon;   // 目录打开图标
        Ref<Texture2D> m_FileIcon;              // 文件图标
    private:
        /// <summary>
        /// 绘制资产节点
        /// </summary>
        /// <param name="node">资产文件节点</param>
        void DrawAssetNode(const std::filesystem::path& fileNode);
    public:
        ProjectAssetsPanel();
        virtual ~ProjectAssetsPanel() override;

        virtual void OnUpdate(DeltaTime dt) override;

        virtual void OnImGuiRender() override;

        virtual void OnEvent(Event& e) override;
    };
}