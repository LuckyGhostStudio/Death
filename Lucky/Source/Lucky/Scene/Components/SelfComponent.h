#pragma once

#include "Component.h"

#include <string>

namespace Lucky
{
    /// <summary>
    /// Self �����Ĭ��������������������
    /// </summary>
    struct SelfComponent
    {
        ComponentType Type = ComponentType::Self;

        std::string Name;   // ���ش������������

        SelfComponent() = default;
        SelfComponent(const SelfComponent&) = default;
        SelfComponent(const std::string& name)
            : Name(name)
        {

        }
    };
}