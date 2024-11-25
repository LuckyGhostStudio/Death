#pragma once

#include "Lucky/Core/Core.h"
#include "Lucky/Scene/Scene.h"
#include "Lucky/Core/Events/Event.h"

namespace Lucky
{
    /// <summary>
    /// 编辑器面板基类
    /// </summary>
    class EditorPanel
    {
    protected:
        bool m_IsFocused = false;
    public:
        virtual ~EditorPanel() = default;

        /// <summary>
        /// 渲染 ImGui 时调用
        /// </summary>
        /// <param name="isOpen">是否打开</param>
        virtual void OnImGuiRender(bool& isOpen) = 0;

        /// <summary>
        /// 事件处理函数
        /// </summary>
        /// <param name="event">事件</param>
        virtual void OnEvent(Event& event) {}

        /// <summary>
        /// 设置 Scene 上下文
        /// </summary>
        /// <param name="context">Scene 上下文</param>
        virtual void SetSceneContext(const Ref<Scene>& context) {}

        bool IsFocused() const { return m_IsFocused; }
    };
}
