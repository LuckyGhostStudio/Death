#pragma once

#include "EditorPanel.h"

#include "Lucky/Scene/Scene.h"
#include "Lucky/Scene/Object.h"

#include "Lucky/Core/Events/KeyEvent.h"

namespace Lucky
{
    /// <summary>
    /// ��������㼶��壺��ʾ�����е������νṹ
    /// </summary>
    class SceneHierarchyPanel : public EditorPanel
    {
    private:
        Ref<Scene> m_Scene;         // �����ʾ�ĳ���
    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene>& scene);

        virtual void SetSceneContext(const Ref<Scene>& scene) override;
        Ref<Scene> GetSceneContext() const { return m_Scene; }

        virtual void OnImGuiRender(bool& isOpen) override;
    private:
        /// <summary>
        /// ����������
        /// </summary>
        /// <param name="Object">����</param>
        void DrawObjectNode(Object Object);

        /// <summary>
        /// ��������ʱ����
        /// </summary>
        void OnDuplicateObject();

        /// <summary>
        /// ɾ������ʱ����
        /// </summary>
        void OnDeleteObject();
    public:
        virtual void OnEvent(Event& event) override;
    private:
        /// <summary>
        /// ��������ʱ����
        /// </summary>
        /// <param name="e">���������¼�</param>
        /// <returns>������</returns>
        bool OnKeyPressed(KeyPressedEvent& e);
    };
}