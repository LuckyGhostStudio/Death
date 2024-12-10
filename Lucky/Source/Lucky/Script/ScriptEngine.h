#pragma once

#include "Lucky/Scene/Scene.h"

#include <filesystem>
#include <string>

extern "C" {
    typedef struct _MonoClass MonoClass;
    typedef struct _MonoObject MonoObject;
    typedef struct _MonoMethod MonoMethod;
    typedef struct _MonoAssembly MonoAssembly;
    typedef struct _MonoImage MonoImage;
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
        friend class ScriptGlue;
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

        static void LoadAppAssembly(const std::filesystem::path& filepath);

        static void OnRuntimeStart(Scene* scene);
        static void OnRuntimeStop();

        /// <summary>
        /// MonoBehaviour 子类脚本是否存在
        /// </summary>
        /// <param name="fullName">namespace.class</param>
        /// <returns></returns>
        static bool MonoBehaviourClassExists(const std::string& fullName);

        /// <summary>
        /// 创建 MonoBehaviour 子类脚本实例
        /// </summary>
        /// <param name="object"></param>
        static void OnCreateMonoBehaviour(Object object);

        /// <summary>
        /// 更新 MonoBehaviour
        /// </summary>
        /// <param name="object">当前对象</param>
        /// <param name="dt">帧间隔</param>
        static void OnUpdateMonoBehaviour(Object object, DeltaTime dt);

        static Scene* GetSceneContext();
        static std::unordered_map<std::string, Ref<ScriptClass>> GetMonoBehaviourClasses();

        static MonoImage* GetCoreAssemblyImage();
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

        /// <summary>
        /// 加载程序集中的类
        /// </summary>
        static void LoadAssemblyClasses();
    };

    /// <summary>
    /// 脚本类：记录 C# 脚本类信息，实例化 和 调用类方法 TODO Temp move to other
    /// </summary>
    class ScriptClass
    {
    private:
        std::string m_ClassNamespace;   // 命名空间名
        std::string m_ClassName;        // 类名

        MonoClass* m_MonoClass = nullptr;   // 对应的 Mono 类
    public:
        ScriptClass() = default;
        ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore = false);

        /// <summary>
        /// 创建 Mono 类实例
        /// </summary>
        /// <returns>实例</returns>
        MonoObject* Instantiate();

        /// <summary>
        /// 获取方法
        /// </summary>
        /// <param name="name">方法名</param>
        /// <param name="parameterCount">参数个数</param>
        /// <returns></returns>
        MonoMethod* GetMethod(const std::string& name, int parameterCount);

        /// <summary>
        /// 调用方法
        /// </summary>
        /// <param name="instance">对象实例</param>
        /// <param name="method">方法</param>
        /// <param name="params">参数列表</param>
        /// <returns></returns>
        MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);
    };

    /// <summary>
    /// 脚本实例：记录 C# 脚本实例的信息，调用实例方法
    /// </summary>
    class ScriptInstance
    {
    private:
        Ref<ScriptClass> m_ScriptClass;     // 脚本类

        MonoObject* m_Instance = nullptr;   // 对应的 Mono 实例

        MonoMethod* m_Constructor = nullptr;    // 构造方法
        MonoMethod* m_AwakeMethod = nullptr;    // Awake 方法
        MonoMethod* m_UpdateMethod = nullptr;   // Update 方法
    public:
        ScriptInstance(Ref<ScriptClass> scriptClass, Object object);

        /// <summary>
        /// 调用 Awake 方法
        /// </summary>
        void InvokeAwake();

        /// <summary>
        /// 调用 Update 方法
        /// </summary>
        /// <param name="dt"></param>
        void InvokeUpdate(float dt);
    };
}