#pragma once

#include <filesystem>
#include <string>

extern "C" {
    typedef struct _MonoClass MonoClass;
    typedef struct _MonoObject MonoObject;
    typedef struct _MonoMethod MonoMethod;
}

namespace Lucky
{
    /// <summary>
    /// 脚本引擎：处理 CSharp Mono 
    /// </summary>
    class ScriptEngine
    {
    private:
        friend class ScriptClass;
    public:
        /// <summary>
        /// 初始化
        /// </summary>
        static void Init();

        /// <summary>
        /// 清理
        /// </summary>
        static void Shutdown();

        /// <summary>
        /// 加载核心程序集
        /// </summary>
        /// <param name="filepath">程序集路径</param>
        static void LoadAssembly(const std::filesystem::path& filepath);
    private:
        /// <summary>
        /// 初始化 Mono 运行时
        /// </summary>
        static void InitMono();

        /// <summary>
        /// 清理 Mono 运行时
        /// </summary>
        static void ShutdownMono();

        /// <summary>
        /// 实例化 Mono 类对象
        /// </summary>
        /// <param name="monoClass">类名</param>
        /// <returns>类对象</returns>
        static MonoObject* InstantiateClass(MonoClass* monoClass);
    };

    /// <summary>
    /// 脚本类：用于创建和实例化 C# 类，调用类方法 TODO Temp move to other
    /// </summary>
    class ScriptClass
    {
    private:
        std::string m_ClassNamespace;   // 命名空间名
        std::string m_ClassName;        // 类名

        MonoClass* m_MonoClass = nullptr;   // 对应的 Mono 类
    public:
        ScriptClass() = default;
        ScriptClass(const std::string& classNamespace, const std::string& className);

        MonoObject* Instantiate();
        MonoMethod* GetMethod(const std::string& name, int parameterCount);
        MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);
    };
}