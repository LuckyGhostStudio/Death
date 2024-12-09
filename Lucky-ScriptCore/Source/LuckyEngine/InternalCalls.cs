using System;
using System.Runtime.CompilerServices;

namespace LuckyEngine
{
    /// <summary>
    /// 内部调用方法：声明 C# 方法 用于调用 Lucky 内的 在 ScriptGlue 注册到 Mono 的 C++ 方法
    /// </summary>
    public static class InternalCalls
    {
        // GameObject
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool GameObject_HasComponent(ulong objectID, Type componentType);

        // Transform
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Transform_GetPosition(ulong objectID, out Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Transform_SetPosition(ulong objectID, ref Vector3 position);

        // Input
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool Input_GetKeyDown(KeyCode keyCode);
    }
}
