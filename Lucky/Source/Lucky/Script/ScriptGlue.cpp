#include "lcpch.h"
#include "ScriptGlue.h"
#include "ScriptEngine.h"

#include "mono/metadata/object.h"

#include "Lucky/Core/Input/Input.h"
#include "Lucky/Core/Input/KeyCodes.h"

#include "Lucky/Scene/Scene.h"
#include "Lucky/Scene/Object.h"

#include "Lucky/Scene/Components/TransformComponent.h"

#include <glm/glm.hpp>

namespace Lucky
{
// 添加内部调用方法(方法名)：C# LuckyEngine.InternalCalls::方法名 调用 C++ 方法名
#define LC_ADD_INTERNAL_CALL(Name) mono_add_internal_call("LuckyEngine.InternalCalls::" #Name, Name)

    /// <summary>
    /// C# 调用
    /// </summary>
    /// <param name="string"></param>
    /// <param name="value"></param>
    static void NativeLog(MonoString* string, int value)
    {
        char* cStr = mono_string_to_utf8(string);
        std::string str(cStr);
        mono_free(cStr);

        std::cout << "String: " << str << ", Parameter: " << value << std::endl;
    }

    /// <summary>
    /// C# 调用
    /// </summary>
    /// <param name="parameter"></param>
    /// <param name="outVec"></param>
    static void NativeLog_Vector(glm::vec3* parameter, glm::vec3* outVec)
    {
        LC_CORE_WARN("Parameter: ({0}, {1}, {2})", parameter->x, parameter->y, parameter->z);

        *outVec = glm::normalize(*parameter);
    }

    /// <summary>
    /// C# 调用
    /// </summary>
    /// <param name="parameter"></param>
    static float NativeLog_VectorRet(glm::vec3* parameter)
    {
        LC_CORE_WARN("Parameter: ({0}, {1}, {2})", parameter->x, parameter->y, parameter->z);

        return glm::dot(*parameter, *parameter);
    }

    static void GameObject_GetPosition(UUID objectID, glm::vec3* outPosition)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        Object object = scene->GetObjectByUUID(objectID);

        *outPosition = object.GetComponent<TransformComponent>().Transform.GetPosition();
    }

    static void GameObject_SetPosition(UUID objectID, glm::vec3* position)
    {
        Scene* scene = ScriptEngine::GetSceneContext();
        Object object = scene->GetObjectByUUID(objectID);

        object.GetComponent<TransformComponent>().Transform.SetPosition(*position);
    }

    static bool Input_GetKeyDown(KeyCode key)
    {
        return Input::IsKeyPressed(key);
    }

    void ScriptGlue::RegisterFunctions()
    {
        // 添加内部调用 (C#)LuckyEngine.InternalCalls::CSharpFunc 调用 (C++)CppFunc
        LC_ADD_INTERNAL_CALL(NativeLog);
        LC_ADD_INTERNAL_CALL(NativeLog_Vector);
        LC_ADD_INTERNAL_CALL(NativeLog_VectorRet);

        LC_ADD_INTERNAL_CALL(GameObject_GetPosition);
        LC_ADD_INTERNAL_CALL(GameObject_SetPosition);

        LC_ADD_INTERNAL_CALL(Input_GetKeyDown);
    }
}
