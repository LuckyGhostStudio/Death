#pragma once

#include "Component.h"

#include <string>

namespace Lucky
{
    /// <summary>
    /// Self 组件（默认组件）：物体固有属性
    /// </summary>
    struct SelfComponent
    {
        ComponentType Type = ComponentType::Self;

        std::string Name;   // 挂载此组件的物体名

        SelfComponent() = default;
        SelfComponent(const SelfComponent&) = default;
        SelfComponent(const std::string& name)
            : Name(name)
        {

        }
    };
}