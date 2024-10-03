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
        std::string ObjectName; // ���ش������������
        bool ObjectEnable;      // ��������״̬

        SelfComponent() = default;
        SelfComponent(const SelfComponent&) = default;
        SelfComponent(const std::string& objectName, bool objectEnable = true)
            : ObjectName(objectName), ObjectEnable(objectEnable) {}
    };
}