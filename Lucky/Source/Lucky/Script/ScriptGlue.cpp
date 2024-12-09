#include "lcpch.h"
#include "ScriptGlue.h"
#include "ScriptEngine.h"

#include "Lucky/Core/Input/Input.h"
#include "Lucky/Core/Input/KeyCodes.h"

#include "Lucky/Scene/Scene.h"
#include "Lucky/Scene/Object.h"

#include "Lucky/Scene/Components/TransformComponent.h"
#include "Lucky/Scene/Components/SpriteRendererComponent.h"
#include "Lucky/Scene/Components/CameraComponent.h"
#include "Lucky/Scene/Components/Rigidbody2DComponent.h"
#include "Lucky/Scene/Components/BoxCollider2DComponent.h"
#include "Lucky/Scene/Components/CircleCollider2DComponent.h"
#include "Lucky/Scene/Components/ScriptComponent.h"

#include <glm/glm.hpp>

#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"

#include "box2d/b2_body.h"

namespace Lucky
{
    // MonoType - Object::HasComponent<>() Map
    static std::unordered_map<MonoType*, std::function<bool(Object)>> s_ObjectHasComponents;

// 添加内部调用方法(方法名)：C# LuckyEngine.InternalCalls::方法名 调用 C++ 方法名
#define LC_ADD_INTERNAL_CALL(Name) mono_add_internal_call("LuckyEngine.InternalCalls::" #Name, Name)

    /// <summary>
    /// objectID 是否存在 componentType 类型组件
    /// </summary>
    /// <param name="objectID">UUID</param>
    /// <param name="componentType">组件类型：Mono 反射类型</param>
    /// <returns></returns>
    static bool GameObject_HasComponent(UUID objectID, MonoReflectionType* componentType)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        LC_CORE_ASSERT(scene, "Scene not found.");
        
        Object object = scene->GetObjectByUUID(objectID);
        LC_CORE_ASSERT(object, "Object not found.");

        MonoType* managedType = mono_reflection_type_get_type(componentType);   // Mono 类型
        LC_CORE_ASSERT(s_ObjectHasComponents.find(managedType) != s_ObjectHasComponents.end(), "managedType not found.");

        return s_ObjectHasComponents.at(managedType)(object);                   // 调用该类型对应的 HasComonent 函数
    }
    
    static void Transform_GetPosition(UUID objectID, glm::vec3* outPosition)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        LC_CORE_ASSERT(scene, "Scene not found.");

        Object object = scene->GetObjectByUUID(objectID);
        LC_CORE_ASSERT(object, "Object not found.");

        *outPosition = object.GetComponent<TransformComponent>().Transform.GetPosition();
    }

    static void Transform_SetPosition(UUID objectID, glm::vec3* position)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        LC_CORE_ASSERT(scene, "Scene not found.");

        Object object = scene->GetObjectByUUID(objectID);
        LC_CORE_ASSERT(object, "Object not found.");

        object.GetComponent<TransformComponent>().Transform.SetPosition(*position);
    }

    static bool Input_GetKeyDown(KeyCode key)
    {
        return Input::IsKeyPressed(key);
    }

    template<typename TComponent>
    static void RegisterComponent()
    {
        // 转换 "Lucky::XXXComponent" 到 "LuckyEngine.XXX" 格式
        std::string_view typeName = typeid(TComponent).name();
        size_t pos = typeName.find_last_of(':');
        std::string_view structName = typeName.substr(pos + 1);
        structName = structName.substr(0, structName.find("Component"));
        std::string managedTypeName = std::format("LuckyEngine.{}", structName);

        // 添加 Mono 组件类型 - HasComponent 到 Map
        MonoType* managedType = mono_reflection_type_from_name(managedTypeName.data(), ScriptEngine::GetCoreAssemblyImage());
        if (!managedType)
        {
            LC_CORE_ERROR("{0} not found.", managedTypeName);
        }
        s_ObjectHasComponents[managedType] = [](Object object) { return object.HasComponent<TComponent>(); };
    }

    void ScriptGlue::RegisterComponents()
    {
        // 注册组件
        RegisterComponent<TransformComponent>();
        RegisterComponent<SpriteRendererComponent>();
        RegisterComponent<CameraComponent>();
        RegisterComponent<Rigidbody2DComponent>();
        RegisterComponent<BoxCollider2DComponent>();
        RegisterComponent<CircleCollider2DComponent>();
        RegisterComponent<ScriptComponent>();
        // TODO 新组件
    }

    void ScriptGlue::RegisterInternalCalls()
    {
        // 添加内部调用 (C#)LuckyEngine.InternalCalls::CSharpFunc 调用 (C++)CppFunc
        LC_ADD_INTERNAL_CALL(GameObject_HasComponent);

        LC_ADD_INTERNAL_CALL(Transform_GetPosition);
        LC_ADD_INTERNAL_CALL(Transform_SetPosition);

        LC_ADD_INTERNAL_CALL(Input_GetKeyDown);
    }
}
