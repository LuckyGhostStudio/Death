#include "lcpch.h"
#include "Scene.h"

#include "Lucky/Renderer/Renderer2D.h"

#include "Components/SelfComponent.h"
#include "Components/TransformComponent.h"
#include "Components/SpriteRendererComponent.h"

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

        object.AddComponent<SelfComponent>(name);       // 添加 Self 组件（默认组件）
        object.AddComponent<TransformComponent>();      // 添加 Transform 组件（默认组件）

        return object;
    }

    void Scene::OnUpdate(DeltaTime dt)
    {
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

        for (auto entity : group)
        {
            const auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

            Renderer2D::DrawQuad(transform.Position, transform.Rotation.z, transform.Scale, sprite.Color);
        }
    }
}