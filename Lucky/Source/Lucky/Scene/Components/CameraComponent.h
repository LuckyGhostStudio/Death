#pragma once

#include "Component.h"
#include "Lucky/Scene/SceneCamera.h"

namespace Lucky
{
    /// <summary>
    /// 相机组件
    /// </summary>
    struct CameraComponent : public Component
    {
        SceneCamera Camera;     // 场景相机

        CameraComponent()
            : Component(ComponentName::Camera) {}

        CameraComponent(const SceneCamera& camera)
            : Component(ComponentName::Camera), Camera(camera) {}

        CameraComponent(const CameraComponent&) = default;
    };
}