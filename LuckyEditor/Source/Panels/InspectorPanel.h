#pragma once

#include "EditorPanel.h"

#include "Lucky/Scene/Object.h"
#include "Lucky/Scene/Selection.h"

#include "Lucky/Renderer/Texture.h"

namespace Lucky
{
    /// <summary>
    /// ������壺��ʾѡ��������ʲ�����Ϣ
    /// </summary>
    class InspectorPanel : public EditorPanel
    {
    private:
        Object& m_SelectionObject = Selection::Object;  // ��ǰѡ����

        Ref<Texture2D> m_SettingsButtonIcon;    // ������ð�ť Icon
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
        InspectorPanel() = default;

        virtual void OnImGuiRender(bool& isOpen) override;
    };
}