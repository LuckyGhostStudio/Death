#pragma once

#include "EditorWindow.h"

#include "Lucky/Scene/Object.h"
#include "Lucky/Scene/Selection.h"

namespace Lucky
{
    /// <summary>
    /// 检视面板：显示选中物体或资产的信息
    /// </summary>
    class InspectorPanel : public EditorWindow
    {
    private:
        Object& m_SelectionObject = Selection::Object;  // 当前选中项
    private:
        /// <summary>
        /// 添加组件 UI
        /// </summary>
        /// <param name="object"></param>
        void AddComponents(Object object);

        /// <summary>
        /// 绘制物体的所有组件 UI
        /// </summary>
        /// <param name="object">物体</param>
        void DrawComponents(Object object);
    public:
        InspectorPanel();
        virtual ~InspectorPanel();

        virtual void OnUpdate(DeltaTime dt) override;

        virtual void OnImGuiRender() override;

        virtual void OnEvent(Event& e) override;
    };
}