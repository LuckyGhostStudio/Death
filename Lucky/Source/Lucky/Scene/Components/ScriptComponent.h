#pragma once

#include "Component.h"

#include <string>

namespace Lucky
{
    /// <summary>
    /// �ű����
    /// </summary>
    struct ScriptComponent
    {
        ComponentType Type = ComponentType::Script;

        std::string ClassNamespace;
        std::string ClassName;
        // TODO �ű��ļ�·��

        ScriptComponent() = default;
        ScriptComponent(const ScriptComponent&) = default;
    };
}