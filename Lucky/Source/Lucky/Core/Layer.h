#pragma once

#include "Lucky/Core/Core.h"
#include "Lucky/Core/DeltaTime.h"
#include "Lucky/Core/Events/Event.h"

namespace Lucky
{
    class Layer
    {
    protected:
        std::string m_DebugName;    // Layer 在 Debug 模式中的名字
    public:
        Layer(const std::string& name = "Layer");

        virtual ~Layer();

        /// <summary>
        /// 将该层添加到层栈时调用
        /// </summary>
        virtual void OnAttach() {}

        /// <summary>
        /// 将该层从层栈移除时调用
        /// </summary>
        virtual void OnDetach() {}

        /// <summary>
        /// 该层更新时调用：每帧调用
        /// </summary>
        /// <param name="dt">帧间隔</param>
        virtual void OnUpdate(DeltaTime dt) {}

        /// <summary>
        /// 渲染ImGui
        /// </summary>
        virtual void OnImGuiRender() {}

        /// <summary>
        /// 事件函数：接收事件
        /// </summary>
        /// <param name="event">事件</param>
        virtual void OnEvent(Event& event) {}

        inline const std::string& GetName() const { return m_DebugName; }
    };
}