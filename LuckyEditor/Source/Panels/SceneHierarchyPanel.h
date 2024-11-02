#pragma once

#include "EditorWindow.h"

#include "Lucky/Core/Core.h"
#include "Lucky/Core/Log.h"

#include "Lucky/Scene/Scene.h"
#include "Lucky/Scene/Object.h"

#include "Lucky/Scene/Selection.h"

namespace Lucky
{
    /// <summary>
    /// ��������㼶��壺��ʾ�����е������νṹ
    /// </summary>
    class SceneHierarchyPanel : public EditorWindow
    {
    private:
        Ref<Scene> m_Scene;                             // �����ʾ�ĳ���
        Object& m_SelectionObject = Selection::Object;  // ��ǰѡ����
    private:
        /// <summary>
        /// ����������
        /// </summary>
        /// <param name="Object">����</param>
        void DrawObjectNode(Object Object);

        /// <summary>
        /// ����������������
        /// </summary>
        /// <param name="object">����</param>
       //void DrawComponents(Object object);
    public:
        SceneHierarchyPanel();
        SceneHierarchyPanel(const Ref<Scene>& scene);
        
        virtual ~SceneHierarchyPanel();

        /// <summary>
        /// ���ó���
        /// </summary>
        /// <param name="scene">����</param>
        void SetScene(const Ref<Scene>& scene);

        virtual void OnUpdate(DeltaTime dt) override;

        virtual void OnImGuiRender() override;

        virtual void OnEvent(Event& e) override;
    };
}