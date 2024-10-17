#include "lcpch.h"
#include "Scene.h"

#include "Lucky/Renderer/Renderer2D.h"

#include "Components/SelfComponent.h"
#include "Components/TransformComponent.h"
#include "Components/SpriteRendererComponent.h"
#include "Components/CameraComponent.h"

#include "Object.h"

namespace Lucky
{
    Scene::Scene(const std::string& name)
        : m_Name(name)
    {

    }

    Scene::~Scene()
    {

    }

    Object Scene::CreateObject(const std::string& name)
    {
        Object object = { m_Registry.create(), this };  // 创建实体

        // std::string n = name + std::to_string((uint32_t)object);
        
        object.AddComponent<SelfComponent>(name);       // 添加 Self 组件（默认组件）
        object.AddComponent<TransformComponent>();      // 添加 Transform 组件（默认组件）

        return object;
    }

    void Scene::DeleteObject(Object object)
    {
        LC_TRACE("当前已删除物体：{0} ", (uint32_t)object);

        m_Registry.destroy(object);
    }

    void Scene::OnUpdate(DeltaTime dt)
    {
        Camera* mainCamera = nullptr;   // 主相机
        Transform cameraTransform;      // 相机 transform

        // 返回有 Transform 和 Camera 的所有实体
        auto cameraView = m_Registry.view<TransformComponent, CameraComponent>();   // 相机实体集合

        for (auto entity : cameraView)
        {
            auto [transform, camera] = cameraView.get<TransformComponent, CameraComponent>(entity);
            
            // 找到主相机
            if (camera.Camera.IsPrimary())
            {
                mainCamera = &camera.Camera;
                cameraTransform = transform.Transform;

                break;
            }
        }

        // 主相机存在
        if (mainCamera)
        {
            // 开始渲染场景
            Renderer2D::BeginScene(*mainCamera, cameraTransform);
            {
                // 返回有 Transform 和 SpriteRenderer 的所有实体
                auto spriteGroup = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);    // Sprite 实体集合

                for (auto entity : spriteGroup)
                {
                    auto [transform, sprite] = spriteGroup.get<TransformComponent, SpriteRendererComponent>(entity);

                    Renderer2D::DrawQuad(transform.Transform, sprite.Color);
                }
            }
            Renderer2D::EndScene(); // 结束渲染场景
        }
    }

    void Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;

        auto cameraView = m_Registry.view<CameraComponent>();   // 所有有 Camera 组件的实体

        for (auto entity : cameraView)
        {
            auto& cameraComponent = cameraView.get<CameraComponent>(entity);    // 获得 Camera 组件
            cameraComponent.Camera.SetViewportSize(width, height);              // 设置视口大小
        }
    }

    template<typename T>
    void Scene::OnComponentAdded(Object object, T& component)
    {
        static_assert(sizeof(T) == 0);
    }

    template<>
    void Scene::OnComponentAdded<SelfComponent>(Object object, SelfComponent& component)
    {

    }

    template<>
    void Scene::OnComponentAdded<TransformComponent>(Object object, TransformComponent& component)
    {

    }

    template<>
    void Scene::OnComponentAdded<CameraComponent>(Object object, CameraComponent& component)
    {
        component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);    // 设置视口
    }

    template<>
    void Scene::OnComponentAdded<SpriteRendererComponent>(Object object, SpriteRendererComponent& component)
    {

    }
}