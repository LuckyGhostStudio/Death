#pragma once

#include "Lucky/Core/DeltaTime.h"
#include "Lucky/Events/Event.h"

#include <string>

namespace Lucky
{
    /// <summary>
    /// 编辑器窗口基类
    /// </summary>
    class EditorWindow
    {
    protected:
        std::string m_Name;     // 名字：窗口 ID
    public:
        EditorWindow(const std::string& name = "EditorWindow");
        virtual ~EditorWindow();

        /// <summary>
        /// 更新：每帧调用
        /// </summary>
        /// <param name="dt">帧间隔</param>
        virtual void OnUpdate(DeltaTime dt) = 0;

        /// <summary>
        /// 渲染 ImGui 时调用
        /// </summary>
        virtual void OnImGuiRender() = 0;

        /// <summary>
        /// 事件处理函数
        /// </summary>
        /// <param name="event">事件</param>
        virtual void OnEvent(Event& event) = 0;

        inline const std::string& GetName() const { return m_Name; }
    };
}