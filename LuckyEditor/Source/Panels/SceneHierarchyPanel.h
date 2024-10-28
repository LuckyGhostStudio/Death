#pragma once

#include "Lucky/Core/Core.h"
#include "Lucky/Core/Log.h"

#include "Lucky/Scene/Scene.h"
#include "Lucky/Scene/Object.h"

#include "Lucky/Scene/Selection.h"

namespace Lucky
{
    /// <summary>
    /// ��������㼶���
    /// </summary>
    class SceneHierarchyPanel
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
        SceneHierarchyPanel() = default;

        /// <summary>
        /// ��������㼶���
        /// </summary>
        /// <param name="context">����</param>
        SceneHierarchyPanel(const Ref<Scene>& scene);

        /// <summary>
        /// ���ó���
        /// </summary>
        /// <param name="context">����</param>
        void SetScene(const Ref<Scene>& scene);

        /// <summary>
        /// ��ȾImGui
        /// </summary>
        void OnImGuiRender();
    };
}