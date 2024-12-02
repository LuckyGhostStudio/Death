#pragma once

namespace Lucky
{
    /// <summary>
    /// 脚本引擎：处理 CSharp Mono 
    /// </summary>
    class ScriptEngine
    {
    public:
        /// <summary>
        /// 初始化
        /// </summary>
        static void Init();

        /// <summary>
        /// 清理
        /// </summary>
        static void Shutdown();
    private:
        /// <summary>
        /// 初始化 Mono 运行时
        /// </summary>
        static void InitMono();

        /// <summary>
        /// 清理 Mono 运行时
        /// </summary>
        static void ShutdownMono();
    };
}