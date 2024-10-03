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
        std::string ObjectName; // 挂载此组件的物体名
        bool ObjectEnable;      // 物体启用状态

        SelfComponent() = default;
        SelfComponent(const SelfComponent&) = default;
        SelfComponent(const std::string& objectName, bool objectEnable = true)
            : ObjectName(objectName), ObjectEnable(objectEnable) {}
    };
}