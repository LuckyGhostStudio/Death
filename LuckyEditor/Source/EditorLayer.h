#pragma once

#include <Lucky.h>

#include "Lucky/Renderer/EditorCamera.h"

#include "Panels/EditorDockSpace.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/InspectorPanel.h"
#include "Panels/SceneViewportPanel.h"
#include "Panels/GameViewportPanel.h"
#include "Panels/RendererStatsPanel.h"
#include "Panels/ProjectAssetsPanel.h"

namespace Lucky
{
    class EditorLayer : public Layer
    {
    private:
        Ref<Framebuffer> m_SceneViewportFramebuffer;    // �����ӿ�֡������
        Ref<Framebuffer> m_GameViewportFramebuffer;     // ��Ϸ�ӿ�֡������

        Ref<Scene> m_ActiveScene;           // �����
        EditorCamera m_EditorCamera;        // �༭�����

        Object m_SquareObject;              // ����������
        Object m_CameraObject;              // �������

        EditorDockSpace m_EditorDockSpace;  // ͣ���ռ�

        // TODO Add PanelManager
        Ref<SceneHierarchyPanel> m_SceneHierarchyPanel; // ���� Hierarchy ���
        Ref<InspectorPanel> m_InspectorPanel;           // �ʲ� Inspector ���
        Ref<SceneViewportPanel> m_SceneViewportPanel;   // ���� Viewport ���
        Ref<GameViewportPanel> m_GameViewportPanel;     // ��Ϸ Viewport ���
        Ref<RendererStatsPanel> m_RendererStatsPanel;   // ���� RendererStats ���
        Ref<ProjectAssetsPanel> m_ProjectAssetsPanel;   // ��Ŀ Assets ���
    public:
        EditorLayer();

        virtual ~EditorLayer() = default;

        /// <summary>
        /// ���ò���ӵ���ջʱ����
        /// </summary>
        virtual void OnAttach() override;

        /// <summary>
        /// ���ò�Ӳ�ջ�Ƴ�ʱ����
        /// </summary>
        virtual void OnDetach() override;

        /// <summary>
        /// ���£�ÿ֡����
        /// </summary>
        /// <param name="dt">֡���</param>
        virtual void OnUpdate(DeltaTime dt) override;

        /// <summary>
        /// ��Ⱦ ImGui
        /// </summary>
        virtual void OnImGuiRender() override;

        /// <summary>
        /// �¼�������
        /// </summary>
        /// <param name="event">�¼�</param>
        virtual void OnEvent(Event& event) override;

        /// <summary>
        /// �����³���
        /// </summary>
        void NewScene();

        /// <summary>
        /// �򿪳���
        /// </summary>
        void OpenScene();

        /// <summary>
        /// �򿪳���
        /// </summary>
        /// <param name="filepath">�ļ�·��</param>
        void OpenScene(const std::filesystem::path& filepath);

        /// <summary>
        /// �������Ϊ
        /// </summary>
        void SaveSceneAs();
    private:
        /// <summary>
        /// ��������ʱ����
        /// </summary>
        /// <param name="e">���������¼�</param>
        /// <returns>������</returns>
        bool OnKeyPressed(KeyPressedEvent& e);

        /// <summary>
        /// ��갴ť����ʱ����
        /// </summary>
        /// <param name="e">��갴ť�����¼�</param>
        /// <returns>������</returns>
        bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
    };
}