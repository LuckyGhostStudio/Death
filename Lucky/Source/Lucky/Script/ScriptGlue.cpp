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

// ����ڲ����÷���(������)��C# LuckyEngine.InternalCalls::������ ���� C++ ������
#define LC_ADD_INTERNAL_CALL(Name) mono_add_internal_call("LuckyEngine.InternalCalls::" #Name, Name)

    /// <summary>
    /// objectID �Ƿ���� componentType �������
    /// </summary>
    /// <param name="objectID">UUID</param>
    /// <param name="componentType">������ͣ�Mono ��������</param>
    /// <returns></returns>
    static bool GameObject_HasComponent(UUID objectID, MonoReflectionType* componentType)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        LC_CORE_ASSERT(scene, "Scene not found.");
        
        Object object = scene->GetObjectByUUID(objectID);
        LC_CORE_ASSERT(object, "Object not found.");

        MonoType* managedType = mono_reflection_type_get_type(componentType);   // Mono ����
        LC_CORE_ASSERT(s_ObjectHasComponents.find(managedType) != s_ObjectHasComponents.end(), "managedType not found.");

        return s_ObjectHasComponents.at(managedType)(object);                   // ���ø����Ͷ�Ӧ�� HasComonent ����
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
        // ת�� "Lucky::XXXComponent" �� "LuckyEngine.XXX" ��ʽ
        std::string_view typeName = typeid(TComponent).name();
        size_t pos = typeName.find_last_of(':');
        std::string_view structName = typeName.substr(pos + 1);
        structName = structName.substr(0, structName.find("Component"));
        std::string managedTypeName = std::format("LuckyEngine.{}", structName);

        // ��� Mono ������� - HasComponent �� Map
        MonoType* managedType = mono_reflection_type_from_name(managedTypeName.data(), ScriptEngine::GetCoreAssemblyImage());
        if (!managedType)
        {
            LC_CORE_ERROR("{0} not found.", managedTypeName);
        }
        s_ObjectHasComponents[managedType] = [](Object object) { return object.HasComponent<TComponent>(); };
    }

    void ScriptGlue::RegisterComponents()
    {
        // ע�����
        RegisterComponent<TransformComponent>();
        RegisterComponent<SpriteRendererComponent>();
        RegisterComponent<CameraComponent>();
        RegisterComponent<Rigidbody2DComponent>();
        RegisterComponent<BoxCollider2DComponent>();
        RegisterComponent<CircleCollider2DComponent>();
        RegisterComponent<ScriptComponent>();
        // TODO �����
    }

    void ScriptGlue::RegisterInternalCalls()
    {
        // ����ڲ����� (C#)LuckyEngine.InternalCalls::CSharpFunc ���� (C++)CppFunc
        LC_ADD_INTERNAL_CALL(GameObject_HasComponent);

        LC_ADD_INTERNAL_CALL(Transform_GetPosition);
        LC_ADD_INTERNAL_CALL(Transform_SetPosition);

        LC_ADD_INTERNAL_CALL(Input_GetKeyDown);
    }
}
