#include "lcpch.h"
#include "Scene.h"

#include "Lucky/Renderer/Renderer2D.h"

#include "Components/SelfComponent.h"
#include "Components/TransformComponent.h"
#include "Components/SpriteRendererComponent.h"
#include "Components/CameraComponent.h"
#include "Components/Rigidbody2DComponent.h"
#include "Components/BoxCollider2DComponent.h"
#include "Components/CircleCollider2DComponent.h"
#include "Components/ScriptComponent.h"

#include "Object.h"

#include "Lucky/Script/ScriptEngine.h"

// Box2D
#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_circle_shape.h"

namespace Lucky
{
    Scene::Scene(const std::string& name)
        : m_Name(name)
    {

    }

    Scene::~Scene()
    {
        delete m_PhysicsWorld;
    }

    /// <summary>
    /// 复制 TComponent 类型的组件：src -> dst 注册表
    /// </summary>
    /// <typeparam name="TComponent">组件类型</typeparam>
    /// <param name="dstRegistry">目标注册表</param>
    /// <param name="srcRegistry">源注册表</param>
    /// <param name="enttMap">UUID - entt 映射表</param>
    template<typename TComponent>
    static void CopyComponent(entt::registry& dstRegistry, entt::registry& srcRegistry, const std::unordered_map<UUID, entt::entity>& enttMap)
    {
        auto view = srcRegistry.view<TComponent>(); // 源注册表所有 TComponent 类型的组件

        for (auto e : view)
        {
            UUID uuid = srcRegistry.get<IDComponent>(e).ID;
            LC_CORE_ASSERT(enttMap.find(uuid) != enttMap.end(), "UUID Not Found in enttMap.");

            entt::entity dstEnttID = enttMap.at(uuid);
            auto& component = srcRegistry.get<TComponent>(e);

            dstRegistry.emplace_or_replace<TComponent>(dstEnttID, component);   // 添加或替换到目标注册表
        }
    }

    /// <summary>
    /// 复制 TComponent 类型的组件：src -> dst 物体
    /// </summary>
    /// <typeparam name="TComponent">组件类型</typeparam>
    /// <param name="dstObj">目标物体</param>
    /// <param name="srcObj">源物体</param>
    template<typename TComponent>
    static void CopyComponentIfExists(Object dstObj, Object srcObj)
    {
        if (srcObj.HasComponent<TComponent>())
        {
            dstObj.AddOrReplaceComponent<TComponent>(srcObj.GetComponent<TComponent>());    // 添加或替换组件到目标物体
        }
    }

    Ref<Scene> Scene::Copy(Ref<Scene> other)
    {
        Ref<Scene> newScene = CreateRef<Scene>();   // 新场景

        // 场景参数 TODO 新参数
        newScene->m_Name = other->m_Name;
        newScene->m_ViewportWidth = other->m_ViewportWidth;
        newScene->m_ViewportHeight = other->m_ViewportHeight;

        auto& srcSceneRegistry = other->m_Registry;     // 待复制场景注册表
        auto& dstSceneRegistry = newScene->m_Registry;  // 新场景注册表

        std::unordered_map<UUID, entt::entity> enttMap; // UUID - entt 映射表

        // 在新场景创建物体
        auto idView = srcSceneRegistry.view<IDComponent>();
        for (auto e : idView)
        {
            UUID uuid = srcSceneRegistry.get<IDComponent>(e).ID;
            const std::string& name = srcSceneRegistry.get<SelfComponent>(e).Name;

            Object newObject = newScene->CreateObject(uuid, name);  // 创建新对象

            enttMap[uuid] = (entt::entity)newObject;    // 添加到 Map
        }

        // 复制 除了 IDComponent SelfComponent 的所有组件：src -> dst 场景注册表
        CopyComponent<TransformComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<SpriteRendererComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<CameraComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<Rigidbody2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<BoxCollider2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<CircleCollider2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        CopyComponent<ScriptComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        // TODO 新组件

        return newScene;
    }

    Object Scene::CreateObject(const std::string& name)
    {
        return CreateObject(UUID(), name);
    }

    Object Scene::CreateObject(UUID uuid, const std::string& name)
    {
        Object object = { m_Registry.create(), this };  // 创建实体

        object.AddComponent<IDComponent>(uuid);     // 添加 ID 组件（默认组件）
        object.AddComponent<SelfComponent>(name);   // 添加 Self 组件（默认组件）
        object.AddComponent<TransformComponent>();  // 添加 Transform 组件（默认组件）

        m_EnttMap[uuid] = object;   // 添加到 EnttMap

        LC_TRACE("Created Object：[ENTT = {0}, UUID {1}, Name {2}]", (uint32_t)object, uuid, name);

        return object;
    }

    void Scene::DeleteObject(Object object)
    {
        LC_TRACE("Removed Object：[ENTT = {0}, UUID {1}, Name {2}]", (uint32_t)object, object.GetUUID(), object.GetName());

        m_Registry.destroy(object);
        m_EnttMap.erase(object.GetUUID());  // 从 EnttMap 移除
    }

    void Scene::OnPhysics2DStart()
    {
        m_PhysicsWorld = new b2World({ 0.0f, -9.8f });

        {
            auto view = m_Registry.view<Rigidbody2DComponent>();    // 所有存在 2D 刚体组件的实体
            for (auto e : view)
            {
                Object object = { e, this };

                //UUID uuid = object.GetComponent<IDComponent>().ID;
                Transform& transform = object.GetComponent<TransformComponent>().Transform;
                Rigidbody2D& rigidbody2D = object.GetComponent<Rigidbody2DComponent>().Rigidbody2d;

                // 2D Body 定义数据
                b2BodyDef bodyDef;

                switch (rigidbody2D.GetBodyType())
                {
                case Rigidbody2D::BodyType::Static:
                    bodyDef.type = b2_staticBody;
                    break;
                case Rigidbody2D::BodyType::Dynamic:
                    bodyDef.type = b2_dynamicBody;
                    break;
                case Rigidbody2D::BodyType::Kinematic:
                    bodyDef.type = b2_kinematicBody;
                    break;
                }

                // TODO 运行时更新 Transform 参数时更新对应的 Rigidbody2D 和 Collier2D 数据
                bodyDef.position.Set(transform.GetPosition().x, transform.GetPosition().y); // 位置
                bodyDef.angle = glm::radians(transform.GetRotation().z);                    // 旋转 deg

                b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);    // 创建 2D Body
                body->SetFixedRotation(rigidbody2D.IsFreezeRotation()); // 设置 固定旋转选项

                b2MassData massData;
                body->GetMassData(&massData);
                massData.mass = rigidbody2D.GetMass();
                body->SetMassData(&massData);                                   // 质量   
                body->SetGravityScale(rigidbody2D.GetGravityScale());           // 线性阻尼
                body->SetLinearDamping(rigidbody2D.GetLinearDrag());            // 角阻尼  
                body->SetAngularDamping(rigidbody2D.GetAngularDrag());          // 重力缩放
                body->SetBullet((bool)rigidbody2D.GetCollisionDetectionMode()); // 碰撞检测模式

                rigidbody2D.SetRuntimeBody(body);  // 设置运行时 Body 数据
            }
        }

        {
            auto view = m_Registry.view<BoxCollider2DComponent>();  // 所有 BoxCollider2D 的实体
            for (auto e : view)
            {
                Object object = { e, this };

                Transform& transform = object.GetComponent<TransformComponent>().Transform;
                BoxCollider2D& boxCollider2D = object.GetComponent<BoxCollider2DComponent>().BoxCollider2d;

                if (object.HasComponent<Rigidbody2DComponent>())
                {
                    Rigidbody2D& rigidbody2D = object.GetComponent<Rigidbody2DComponent>().Rigidbody2d;

                    LC_CORE_ASSERT(rigidbody2D.GetRuntimeBody(), "RuntimeBody is null.");

                    b2Body* body = static_cast<b2Body*>(rigidbody2D.GetRuntimeBody());  // 运行时 Body 数据

                    // 2D 多边形形状数据
                    b2PolygonShape boxShape;
                    boxShape.SetAsBox(boxCollider2D.GetSize().x * 0.5f * glm::abs(transform.GetScale().x), boxCollider2D.GetSize().y * 0.5f * glm::abs(transform.GetScale().y), b2Vec2(boxCollider2D.GetOffset().x, boxCollider2D.GetOffset().y), 0.0f);

                    // 2D Fixture 定义数据：物理对象的数据 TODO Physics Material
                    b2FixtureDef fixtureDef;
                    fixtureDef.shape = &boxShape;
                    fixtureDef.density = boxCollider2D.GetDensity();
                    fixtureDef.friction = boxCollider2D.GetFriction();
                    //fixtureDef.restitution = boxCollider2D.GetRestitution();
                    //fixtureDef.restitutionThreshold = boxCollider2D.GetRestitutionThreshold();
                    body->CreateFixture(&fixtureDef);   // 创建 Fixture

                    boxCollider2D.SetRuntimeFixture(&fixtureDef);   // 设置运行时 Fixture 数据
                }
            }
        }

        {
            auto view = m_Registry.view<CircleCollider2DComponent>();  // 所有 CircleCollider2D 的实体
            for (auto e : view)
            {
                Object object = { e, this };

                Transform& transform = object.GetComponent<TransformComponent>().Transform;
                CircleCollider2D& circleCollider2D = object.GetComponent<CircleCollider2DComponent>().CircleCollider2d;

                if (object.HasComponent<Rigidbody2DComponent>())
                {
                    Rigidbody2D& rigidbody2D = object.GetComponent<Rigidbody2DComponent>().Rigidbody2d;

                    LC_CORE_ASSERT(rigidbody2D.GetRuntimeBody(), "RuntimeBody is null.");

                    b2Body* body = static_cast<b2Body*>(rigidbody2D.GetRuntimeBody());  // 运行时 Body 数据

                    // 2D 圆形形状数据
                    b2CircleShape circleShape;
                    circleShape.m_p.Set(circleCollider2D.GetOffset().x, circleCollider2D.GetOffset().y);
                    circleShape.m_radius = glm::max(glm::abs(transform.GetScale().x), glm::abs(transform.GetScale().y)) * circleCollider2D.GetRadius();

                    // 2D Fixture 定义数据：物理对象的数据 TODO Physics Material
                    b2FixtureDef fixtureDef;
                    fixtureDef.shape = &circleShape;
                    fixtureDef.density = circleCollider2D.GetDensity();
                    fixtureDef.friction = circleCollider2D.GetFriction();
                    //fixtureDef.restitution = circleCollider2D.GetRestitution();
                    //fixtureDef.restitutionThreshold = circleCollider2D.GetRestitutionThreshold();
                    body->CreateFixture(&fixtureDef);   // 创建 Fixture

                    circleCollider2D.SetRuntimeFixture(&fixtureDef);   // 设置运行时 Fixture 数据
                }
            }
        }
    }

    void Scene::OnPhysics2DStop()
    {
        delete m_PhysicsWorld;
        m_PhysicsWorld = nullptr;
    }

    void Scene::OnRuntimeStart()
    {
        OnPhysics2DStart(); // 开始 2D 物理

        // Scripting
        {
            ScriptEngine::OnRuntimeStart(this); // 开始运行脚本

            // 实例化所有脚本
            auto view = m_Registry.view<ScriptComponent>(); // 所有 ScriptComponent 的实体
            for (auto e : view)
            {
                Object object = { e, this };

                const ScriptComponent& scriptComponent = object.GetComponent<ScriptComponent>();

                ScriptEngine::OnCreateMonoBehaviour(object);    // 创建 MonoBehaviour 子类实例
            }
        }
    }

    void Scene::OnRuntimeStop()
    {
        OnPhysics2DStop();  // 停止 2D 物理

        ScriptEngine::OnRuntimeStop();      // 脚本停止运行
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

    void Scene::OnUpdateEditor(DeltaTime dt)
    {
        Camera* mainCamera = nullptr;   // 主相机
        Transform cameraTransform;      // 相机 transform

        // 返回有 Transform 和 Camera 的所有实体
        auto cameraView = m_Registry.view<TransformComponent, CameraComponent>();   // 相机实体集合

        for (auto entity : cameraView)
        {
            auto [transform, camera] = cameraView.get<TransformComponent, CameraComponent>(entity);

            // 找到主相机（第一个）
            if (camera.Primary)
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

    void Scene::OnUpdateRuntime(DeltaTime dt)
    {
        // C# Script Update
        {
            auto view = m_Registry.view<ScriptComponent>(); // 所有 ScriptComponent 的实体
            for (auto e : view)
            {
                Object object = { e, this };

                ScriptEngine::OnUpdateMonoBehaviour(object, dt);    // 调用 MonoBehaviour.Update
            }
        }

        // Physics 2D
        {
            const int32_t velocityIterations = 6;   // 速度迭代次数
            const int32_t positionIterations = 2;   // 位置迭代次数

            m_PhysicsWorld->Step(dt, velocityIterations, positionIterations);

            auto rigidbody2DView = m_Registry.view<Rigidbody2DComponent>();
            for (auto e : rigidbody2DView)
            {
                Object object = { e, this };

                Transform& transform = object.GetComponent<TransformComponent>().Transform;
                Rigidbody2D& rigidbody2D = object.GetComponent<Rigidbody2DComponent>().Rigidbody2d;

                b2Body* body = (b2Body*)rigidbody2D.GetRuntimeBody();   // 运行时 Body 数据

                // 从 Box2D 更新 Transform 数据
                const auto& position = body->GetPosition();
                transform.GetPosition().x = position.x;
                transform.GetPosition().y = position.y;
                transform.GetRotation().z = glm::degrees(body->GetAngle()); // deg
            }
        }

        // Runtime: 物理 脚本
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

    Object Scene::DuplicateObject(Object object)
    {
        std::string name = object.GetName();
        Object newObject = CreateObject(name);  // 创建新物体

        // 复制 除了 IDComponent SelfComponent 的所有组件：src -> dst 物体
        CopyComponentIfExists<TransformComponent>(newObject, object);
        CopyComponentIfExists<SpriteRendererComponent>(newObject, object);
        CopyComponentIfExists<CameraComponent>(newObject, object);
        CopyComponentIfExists<Rigidbody2DComponent>(newObject, object);
        CopyComponentIfExists<BoxCollider2DComponent>(newObject, object);
        CopyComponentIfExists<CircleCollider2DComponent>(newObject, object);
        CopyComponentIfExists<ScriptComponent>(newObject, object);
        // TODO 新组件

        LC_TRACE("Copied Object：[ENTT = {0}, UUID {1}, Name {2}] -> [ENTT = {3}, UUID {4}, Name {5}]", (uint32_t)object, object.GetUUID(), name, (uint32_t)newObject, newObject.GetUUID(), name);

        return newObject;
    }

    Object Scene::GetObjectByUUID(UUID uuid)
    {
        // TODO Assert
        if (m_EnttMap.find(uuid) != m_EnttMap.end())
        {
            return { m_EnttMap.at(uuid), this };
        }

        return {};
    }

    Object Scene::GetPrimaryCameraObject()
    {
        auto view = m_Registry.view<CameraComponent>();  // 返回有 Camera 组件的物体
        
        for (auto object : view)
        {
            const auto& camera = view.get<CameraComponent>(object);  // Camera
            // 主相机
            if (camera.Primary)
            {
                return Object{ object, this };  // 相机实体
            }
        }
        
        return {};
    }

    template<typename TComponent>
    void Scene::OnComponentAdded(Object object, TComponent& component)
    {
        static_assert(sizeof(TComponent) == 0);
    }

    template<>
    void Scene::OnComponentAdded<IDComponent>(Object object, IDComponent& component)
    {

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
    
    template<>
    void Scene::OnComponentAdded<CircleCollider2DComponent>(Object object, CircleCollider2DComponent& component)
    {
    }
    
    template<>
    void Scene::OnComponentAdded<ScriptComponent>(Object object, ScriptComponent& component)
    {
    }

    // TODO 添加新组件
}