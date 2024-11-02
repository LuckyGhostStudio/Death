#pragma once

#include "EditorWindow.h"

#include "Lucky/Scene/Object.h"
#include "Lucky/Scene/Selection.h"

namespace Lucky
{
    /// <summary>
    /// ������壺��ʾѡ��������ʲ�����Ϣ
    /// </summary>
    class InspectorPanel : public EditorWindow
    {
    private:
        Object& m_SelectionObject = Selection::Object;  // ��ǰѡ����
    private:
        /// <summary>
        /// ������ UI
        /// </summary>
        /// <param name="object"></param>
        void AddComponents(Object object);

        /// <summary>
        /// ���������������� UI
        /// </summary>
        /// <param name="object">����</param>
        void DrawComponents(Object object);
    public:
        InspectorPanel();
        virtual ~InspectorPanel();

        virtual void OnUpdate(DeltaTime dt) override;

        virtual void OnImGuiRender() override;

        virtual void OnEvent(Event& e) override;
    };
}