#pragma once

namespace Lucky
{
    /// <summary>
    /// 脚本注册表：注册 C++ 方法实现 由在 InternalCalls.cs 声明的 C# 方法调用 
    /// </summary>
    class ScriptGlue
    {
    public:
        /// <summary>
        /// 注册函数
        /// </summary>
        static void RegisterFunctions();
    };
}