#pragma once

#include "Lucky/Core/Core.h"
#include "Lucky/Core/Log.h"

#include "Lucky/Scene/Object.h"

namespace Lucky
{
    /// <summary>
    /// ����������� TODO �޸�Ϊ Inspector
    /// </summary>
    class ObjectPropertiesPanel
    {
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
        ObjectPropertiesPanel() = default;

        /// <summary>
        /// ��ȾImGui
        /// </summary>
        void OnImGuiRender();
    };
}