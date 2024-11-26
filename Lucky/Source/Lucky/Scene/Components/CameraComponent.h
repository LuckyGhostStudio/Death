#pragma once

#include "Component.h"
#include "Lucky/Scene/SceneCamera.h"

namespace Lucky
{
    /// <summary>
    /// 相机组件
    /// </summary>
    struct CameraComponent
    {
        ComponentType Type = ComponentType::Camera;

        SceneCamera Camera;     // 场景相机

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
        CameraComponent(const SceneCamera& camera)
            : Camera(camera)
        {

        }
    };
}