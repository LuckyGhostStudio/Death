#pragma once

#include "Component.h"

#include <string>

namespace Lucky
{
    /// <summary>
    /// 脚本组件
    /// </summary>
    struct ScriptComponent
    {
        ComponentType Type = ComponentType::Script;

        std::string ClassNamespace;
        std::string ClassName;
        // TODO 脚本文件路径

        ScriptComponent() = default;
        ScriptComponent(const ScriptComponent&) = default;
    };
}