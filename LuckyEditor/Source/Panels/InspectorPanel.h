#pragma once

#include "EditorWindow.h"

#include "Lucky/Core/Core.h"
#include "Lucky/Core/Log.h"

#include "Lucky/Scene/Object.h"

namespace Lucky
{
    /// <summary>
    /// ������壺��ʾѡ��������ʲ�����Ϣ
    /// </summary>
    class InspectorPanel : public EditorWindow
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
        InspectorPanel();
        virtual ~InspectorPanel();

        /// <summary>
        /// ��ȾImGui
        /// </summary>
        void OnImGuiRender();
    };
}