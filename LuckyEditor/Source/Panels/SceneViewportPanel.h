#pragma once

#include "EditorWindow.h"

#include "Lucky/Renderer/Framebuffer.h"
#include "Lucky/Renderer/EditorCamera.h"
#include "Lucky/Scene/Scene.h"
#include "Lucky/Scene/Selection.h"

#include "Lucky/Events/KeyEvent.h"
#include "Lucky/Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace Lucky
{
    /// <summary>
    /// �����ӿ���壺��Ⱦ�������壨�༭������Ļ��棩
    /// </summary>
    class SceneViewportPanel : public EditorWindow
    {
    private:
        Ref<Framebuffer> m_Framebuffer;     // ֡������
        Ref<Scene> m_Scene;                 // ����� �����
        EditorCamera m_EditorCamera;        // �༭����� ����²��ֲ���

        Object m_PickedObject;              // �����ʰȡ������

        glm::vec2 m_Bounds[2];              // �ӿڱ߽磨���Ͻǣ����½ǣ�
        glm::vec2 m_Size = { 0.0f, 0.0f };  // �ӿڴ�С���� m_Bounds ����

        bool m_Focused = false;     // �ӿڱ��۽� +
        bool m_Hovered = false;     // �ӿڱ������ͣ +

        int m_GizmoType = -1;       // Gizmo �������� -1�� 0ƽ�� 1��ת 2���� + TODO class Gizmo
    public:
        SceneViewportPanel();
        SceneViewportPanel(const Ref<Framebuffer>& framebuffer, const Ref<Scene>& scene);
        virtual ~SceneViewportPanel() override;

        /// <summary>
        /// ���ó���
        /// </summary>
        /// <param name="scene">����</param>
        void SetScene(const Ref<Scene>& scene);

        virtual void OnUpdate(DeltaTime dt) override;

        virtual void OnImGuiRender() override;

        /// <summary>
        /// �¼�������
        /// </summary>
        /// <param name="e">�¼�</param>
        virtual void OnEvent(Event& e) override;
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