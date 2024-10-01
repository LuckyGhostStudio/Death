#include "lcpch.h"
#include "Scene.h"

#include "Lucky/Renderer/Renderer2D.h"

#include "Lucky/Scene/Components/Transform.h"
#include "Lucky/Scene/Components/SpriteRenderer.h"

namespace Lucky
{
    Scene::Scene()
    {

    }

    Scene::~Scene()
    {

    }

    entt::entity Scene::CreateEntity()
    {
        return m_Registry.create();     // 创建实体到注册表
    }

    void Scene::OnUpdate(DeltaTime dt)
    {
        auto group = m_Registry.group<Transform>(entt::get<SpriteRenderer>);

        for (auto entity : group)
        {
            const auto& [transform, sprite] = group.get<Transform, SpriteRenderer>(entity);

            Renderer2D::DrawQuad(transform.GetPosition(), transform.GetRotation().z, transform.GetScale(), sprite.GetColor());
        }
    }
}