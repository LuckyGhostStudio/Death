#pragma once

#include "Component.h"
#include "Lucky/Scene/SceneCamera.h"

namespace Lucky
{
    /// <summary>
    /// ������
    /// </summary>
    struct CameraComponent
    {
        ComponentType Type = ComponentType::Camera;

        SceneCamera Camera;     // �������

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
        CameraComponent(const SceneCamera& camera)
            : Camera(camera)
        {

        }
    };
}