using System;
using System.Runtime.CompilerServices;

namespace LuckyEngine
{
    public struct Vector3
    {
        public float x, y, z;

        public static Vector3 Zero => new Vector3(0.0f);

        public Vector3(float scalar)
        {
            x = scalar;
            y = scalar;
            z = scalar;
        }

        public Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        public static Vector3 operator +(Vector3 a, Vector3 b)
        {
            return new Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
        }

        public static Vector3 operator -(Vector3 a, Vector3 b)
        {
            return new Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
        }

        public static Vector3 operator *(Vector3 vector, float scalar)
        {
            return new Vector3(vector.x * scalar, vector.y * scalar, vector.z * scalar);
        }

        public static Vector3 operator *(float scalar, Vector3 vector)
        {
            return new Vector3(vector.x * scalar, vector.y * scalar, vector.z * scalar);
        }
    }

    /// <summary>
    /// 内部调用方法：声明 C# 方法 用于调用 Lucky 内的 在 ScriptGlue 注册到 Mono 的 C++ 方法
    /// </summary>
    public static class InternalCalls
    {
        // 内部调用：调用 Lucky 中的 C++ 方法
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void NativeLog(string text, int parameter);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void NativeLog_Vector(ref Vector3 parameter, out Vector3 result);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float NativeLog_VectorRet(ref Vector3 parameter);

        // GameObject

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void GameObject_GetPosition(ulong objectID, out Vector3 position);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void GameObject_SetPosition(ulong objectID, ref Vector3 position);

        // Input
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool Input_GetKeyDown(KeyCode keyCode);
    }

    public class MonoBehaviour
    {
        protected MonoBehaviour()
        {
            ID = 0;
        }

        internal MonoBehaviour(ulong id)
        {
            ID = id;
        }

        // TODO Transform

        public readonly ulong ID;

        public Vector3 Position
        {
            get
            {
                InternalCalls.GameObject_GetPosition(ID, out Vector3 position);
                return position;
            }
            set
            {
                InternalCalls.GameObject_SetPosition(ID, ref value);
            }
        }
    }

    public class GameObject
    {
        
    }
}
