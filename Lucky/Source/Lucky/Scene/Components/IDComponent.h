#pragma once

#include "Component.h"
#include "Lucky/Core/UUID.h"

namespace Lucky
{
    /// <summary>
    /// ID ×é¼þ
    /// </summary>
    struct IDComponent
    {
        ComponentType Type = ComponentType::ID; // Temp TODO Remove

        UUID ID;

        IDComponent() = default;
        IDComponent(const IDComponent&) = default;
        IDComponent(const UUID& uuid)
            : ID(uuid)
        {

        }
    };
}