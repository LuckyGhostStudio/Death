#pragma once

#include "Component.h"
#include "Lucky/Scene/SceneCamera.h"

namespace Lucky
{
    /// <summary>
    /// ������
    /// </summary>
    struct CameraComponent : public Component
    {
        SceneCamera Camera;     // �������

        CameraComponent()
            : Component(ComponentName::Camera) {}

        CameraComponent(const SceneCamera& camera)
            : Component(ComponentName::Camera), Camera(camera) {}

        CameraComponent(const CameraComponent&) = default;
    };
}