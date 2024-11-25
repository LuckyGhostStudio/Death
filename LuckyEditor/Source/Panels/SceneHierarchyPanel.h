#pragma once

#include "EditorPanel.h"

#include "Lucky/Scene/Scene.h"
#include "Lucky/Scene/Object.h"

namespace Lucky
{
    /// <summary>
    /// ��������㼶��壺��ʾ�����е������νṹ
    /// </summary>
    class SceneHierarchyPanel : public EditorPanel
    {
    private:
        Ref<Scene> m_Scene;         // �����ʾ�ĳ���
    private:
        /// <summary>
        /// ����������
        /// </summary>
        /// <param name="Object">����</param>
        void DrawObjectNode(Object Object);
    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene>& scene);

        virtual void SetSceneContext(const Ref<Scene>& scene) override;
        Ref<Scene> GetSceneContext() const { return m_Scene; }

        virtual void OnImGuiRender(bool& isOpen) override;

        virtual void OnEvent(Event& event) override;
    };
}