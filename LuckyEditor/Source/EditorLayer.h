#pragma once

#include <Lucky.h>

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ObjectPropertiesPanel.h"

namespace Lucky
{
    class EditorLayer : public Layer
    {
    private:
        ShaderLibrary m_ShaderLibrary;          // ��ɫ����

        Ref<Texture2D> m_CheckerboardTexture;   // ��������

        Ref<Framebuffer> m_Framebuffer;         // ֡������

        Ref<Scene> m_ActiveScene;           // �����
        Object m_SquareObject;              // ����������
        Object m_CameraObject;              // �������

        bool m_ViewportFocused = false;     // �ӿڱ��۽�
        bool m_ViewportHovered = false;     // �����ͣ���ӿ�

        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };  // �ӿڴ�С

        SceneHierarchyPanel m_HierarchyPanel;       // ���� Hierarchy ���
        ObjectPropertiesPanel m_PropertiesPanel;    // �����������

        float fps = 0.0f;
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
    };
}