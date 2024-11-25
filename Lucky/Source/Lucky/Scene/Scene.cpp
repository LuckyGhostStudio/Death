#include "lcpch.h"
#include "Scene.h"

#include "Lucky/Renderer/Renderer2D.h"

#include "Components/SelfComponent.h"
#include "Components/TransformComponent.h"
#include "Components/SpriteRendererComponent.h"
#include "Components/CameraComponent.h"
#include "Components/Rigidbody2DComponent.h"
#include "Components/BoxCollider2DComponent.h"

#include "Object.h"

// Box2D
#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"

namespace Lucky
{
    // TODO Temp
    static b2BodyType Rigidbody2DTypeToBox2DBody(Rigidbody2D::BodyType bodyType)
    {
        switch (bodyType)
        {
            case Rigidbody2D::BodyType::Static:    return b2_staticBody;
            case Rigidbody2D::BodyType::Dynamic:   return b2_dynamicBody;
            case Rigidbody2D::BodyType::Kinematic: return b2_kinematicBody;
        }

        LC_CORE_ASSERT(false, "Unknown body type");

        return b2_staticBody;
    }

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

    void Scene::OnRuntimeStart()
    {
        m_PhysicsWorld = new b2World({ 0.0f, -9.8f });

        auto rb2DView = m_Registry.view<Rigidbody2DComponent>();    // 所有存在 2D 刚体组件的实体

        for (auto e : rb2DView)
        {
            Object object = { e, this };

            Transform& transform = object.GetComponent<TransformComponent>().Transform;
            Rigidbody2D& rb2d = object.GetComponent<Rigidbody2DComponent>().Rigidbody2d;

            // 2D Body 定义数据
            b2BodyDef bodyDef;
            bodyDef.type = Rigidbody2DTypeToBox2DBody(rb2d.GetBodyType());              // 类型 TODO 运行时更改
            bodyDef.position.Set(transform.GetPosition().x, transform.GetPosition().y); // 位置
            bodyDef.angle = transform.GetRotation().z;                                  // 旋转

            b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);    // 创建 2D Body
            body->SetFixedRotation(rb2d.IsFreezeRotation());        // 设置 固定旋转选项

            rb2d.SetRuntimeBody(body);  // 设置运行时 Body 数据

            if (object.HasComponent<BoxCollider2DComponent>())
            {
                BoxCollider2D& bc2d = object.GetComponent<BoxCollider2DComponent>().BoxCollider2d;

                // 2D 形状数据
                b2PolygonShape boxShape;
                boxShape.SetAsBox(bc2d.GetSize().x * transform.GetScale().x, bc2d.GetSize().y * transform.GetScale().y);

                // 2D Fixture 定义数据：物理对象的数据
                b2FixtureDef fixtureDef;
                fixtureDef.shape = &boxShape;
                fixtureDef.density = bc2d.GetDensity();
                fixtureDef.friction = bc2d.GetFriction();
                fixtureDef.restitution = bc2d.GetRestitution();
                fixtureDef.restitutionThreshold = bc2d.GetRestitutionThreshold();

                body->CreateFixture(&fixtureDef);   // 创建 Fixture
            }
        }
    }

    void Scene::OnRuntimeStop()
    {
        delete m_PhysicsWorld;
        m_PhysicsWorld = nullptr;
    }

    void Scene::OnUpdateEditor(DeltaTime dt, EditorCamera& camera)
    {
        Renderer2D::BeginScene(camera);
        {
            auto spriteGroup = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

            for (auto entity : spriteGroup)
            {
                auto [transform, sprite] = spriteGroup.get<TransformComponent, SpriteRendererComponent>(entity);

                Renderer2D::DrawQuad(transform.Transform, sprite.Color, sprite.Sprite, (int)entity);
            }
        }
        Renderer2D::EndScene();
    }

    void Scene::OnUpdateRuntime(DeltaTime dt)
    {
        // Physics
        {
            const int32_t velocityIterations = 6;   // 速度迭代次数
            const int32_t positionIterations = 2;   // 位置迭代次数

            m_PhysicsWorld->Step(dt, velocityIterations, positionIterations);

            auto rb2DView = m_Registry.view<Rigidbody2DComponent>();
            for (auto e : rb2DView)
            {
                Object object = { e, this };

                Transform& transform = object.GetComponent<TransformComponent>().Transform;
                Rigidbody2D& rb2d = object.GetComponent<Rigidbody2DComponent>().Rigidbody2d;

                b2Body* body = (b2Body*)rb2d.GetRuntimeBody();  // 运行时 Body 数据

                // 从 Box2D 更新 Transform 数据
                const auto& position = body->GetPosition();
                transform.GetPosition().x = position.x;
                transform.GetPosition().y = position.y;
                transform.GetRotation().z = glm::degrees(body->GetAngle());

                LC_TRACE("PositionY: {0}", position.y);
            }
        }

        Camera* mainCamera = nullptr;   // 主相机
        Transform cameraTransform;      // 相机 transform

        // 返回有 Transform 和 Camera 的所有实体
        auto cameraView = m_Registry.view<TransformComponent, CameraComponent>();   // 相机实体集合

        for (auto entity : cameraView)
        {
            auto [transform, camera] = cameraView.get<TransformComponent, CameraComponent>(entity);
            
            // 找到主相机（第一个）
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

                    Renderer2D::DrawQuad(transform.Transform, sprite.Color, sprite.Sprite, (int)entity);
                }
            }
            Renderer2D::EndScene(); // 结束渲染场景
        }
    }

    void Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;
        // TODO 修改为更新 Camera 视口
        auto cameraView = m_Registry.view<CameraComponent>();   // 所有有 Camera 组件的实体

        for (auto entity : cameraView)
        {
            auto& cameraComponent = cameraView.get<CameraComponent>(entity);    // 获得 Camera 组件
            cameraComponent.Camera.SetViewportSize(width, height);              // 设置视口大小
        }
    }

    Object Scene::GetPrimaryCameraObject()
    {
        auto view = m_Registry.view<CameraComponent>();  // 返回有 Camera 组件的物体
        
        for (auto object : view)
        {
            const auto& camera = view.get<CameraComponent>(object).Camera;  // Camera
            // 主相机
            if (camera.IsPrimary())
            {
                return Object{ object, this };  // 相机实体
            }
        }
        
        return {};
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
        if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
        {
            component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);    // 设置视口
        }
    }

    template<>
    void Scene::OnComponentAdded<SpriteRendererComponent>(Object object, SpriteRendererComponent& component)
    {

    }

    template<>
    void Scene::OnComponentAdded<Rigidbody2DComponent>(Object object, Rigidbody2DComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<BoxCollider2DComponent>(Object object, BoxCollider2DComponent& component)
    {
    }
}