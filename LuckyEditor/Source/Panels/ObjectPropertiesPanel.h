#pragma once

#include "Lucky/Core/Core.h"
#include "Lucky/Core/Log.h"

#include "Lucky/Scene/Object.h"
#include "Selection.h"

namespace Lucky
{
    /// <summary>
    /// ����������� TODO �޸�Ϊ Inspector
    /// </summary>
    class ObjectPropertiesPanel
    {
    private:
        Selection m_Selection;  // ��ǰѡ����
    private:
        /// <summary>
        /// ����������������
        /// </summary>
        /// <param name="object">����</param>
        void DrawComponents(Object object);
    public:
        ObjectPropertiesPanel() = default;

        ObjectPropertiesPanel(Selection selection);

        /// <summary>
        /// ����ѡ����
        /// </summary>
        /// <param name="selection">ѡ����</param>
        void SetSelection(Selection selection);

        /// <summary>
        /// ��ȾImGui
        /// </summary>
        void OnImGuiRender();
    };
}