#pragma once

namespace Lucky
{
    /// <summary>
    /// 组件名称
    /// </summary>
    enum class ComponentName
    {
        None = 0,

        Self,

        Transform,
        SpriteRenderer,
    };

    /// <summary>
    /// 组件基类
    /// </summary>
    struct Component
    {
        ComponentName Name; // 组件名
        bool Enable;        // 启用状态
        bool IsDefault;     // 是否为默认组件

        Component() = default;
        Component(const Component&) = default;

        /// <summary>
        /// 组件
        /// </summary>
        /// <param name="name">组件名</param>
        /// <param name="enable">启用状态</param>
        /// <param name="isDefault">是否为默认组件</param>
        Component(ComponentName name = ComponentName::None, bool isDefault = false)
            : Name(name), Enable(true), IsDefault(isDefault) {}

        virtual ~Component() = default;
    };
}