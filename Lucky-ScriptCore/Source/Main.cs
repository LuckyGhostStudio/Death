using System;
using System.Runtime.CompilerServices;

namespace LuckyEngine
{
    public struct Vector3
    {
        public float X;
        public float Y;
        public float Z;

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
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
    }

    public class GameObject
    {
        public float FloatVar { get; set; }

        public GameObject()
        {
            Console.WriteLine("Main Constructor.");
            Log("LuckyGhostStudio", 123);

            Vector3 pos = new Vector3(5, 2, 0);
            Vector3 result = Log(pos);

            Console.WriteLine($"{result.X}, {result.Y}, {result.Z}");
            Console.WriteLine("{0}", InternalCalls.NativeLog_VectorRet(ref pos));
        }

        // 由 C++ 通过 Mono 调用

        public void PrintMessage()
        {
            Console.WriteLine("Hello LuckyEngine from C#.");
        }

        public void PrintInt(int value)
        {
            Console.WriteLine($"C#: {value}");
        }

        public void PrintInts(int value1, int value2)
        {
            Console.WriteLine($"C#: {value1} and {value2}");
        }

        public void PrintCustomMessage(string message)
        {
            Console.WriteLine($"C#: {message}");
        }

        private void Log(string text, int parameter)
        {
            InternalCalls.NativeLog(text, parameter);
        }

        private Vector3 Log(Vector3 parameter)
        {
            InternalCalls.NativeLog_Vector(ref parameter, out Vector3 result);
            return result;
        }
    }
}
