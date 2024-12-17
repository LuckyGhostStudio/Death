#pragma once

#include "Lucky/Scene/Scene.h"

#include <filesystem>
#include <string>
#include <map>

extern "C" {
    typedef struct _MonoClass MonoClass;
    typedef struct _MonoObject MonoObject;
    typedef struct _MonoMethod MonoMethod;
    typedef struct _MonoAssembly MonoAssembly;
    typedef struct _MonoImage MonoImage;
    typedef struct _MonoClassField MonoClassField;
}

namespace Lucky
{
    enum class ScriptFieldType
    {
        None = 0,

        Float,
        Double,

        Bool,
        SByte,
        Short,
        Int,
        Long,

        Byte,
        UShort,
        UInt,
        ULong, 

        Char,
        String,

        Vector2,
        Vector3,
        Vector4,

        GameObject
    };

    /// <summary>
    /// 脚本字段
    /// </summary>
    struct ScriptField
    {
        ScriptFieldType Type;   // 类型
        std::string Name;       // 名称

        MonoClassField* ClassField;
    };

    /// <summary>
    /// 脚本字段实例 ScriptField + Data
    /// </summary>
    struct ScriptFieldInstance
    {
    private:
        friend class ScriptEngine;
        friend class ScriptInstance;
    public:
        ScriptField Field;

        ScriptFieldInstance()
        {
            memset(m_ValueBuffer, 0, sizeof(m_ValueBuffer));
        }
    private:
        uint8_t m_ValueBuffer[16];  // 字段值 Buffer
    public:
        template<typename T>
        T GetValue()
        {
            static_assert(sizeof(T) <= 16, "Type too large.");
            return *(T*)m_ValueBuffer;
        }

        template<typename T>
        void SetValue(T value)
        {
            static_assert(sizeof(T) <= 16, "Type too large.");
            memcpy(m_ValueBuffer, &value, sizeof(T));
        }
    };

    using ScriptFieldMap = std::unordered_map<std::string, ScriptFieldInstance>;    // 字段名 - 字段实例

    class ScriptInstance;

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
        static Ref<ScriptInstance> GetMonoBehaviourScriptInstance(UUID objectID);
        static Ref<ScriptClass> GetMonoBehaviourClass(const std::string& name);
        static std::unordered_map<std::string, Ref<ScriptClass>> GetMonoBehaviourClasses();
        static ScriptFieldMap& GetScriptFieldMap(UUID objectID);

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
        friend class ScriptEngine;
    private:
        std::string m_ClassNamespace;   // 命名空间名
        std::string m_ClassName;        // 类名

        std::map<std::string, ScriptField> m_Fields;  // 所有字段 Map

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

        const std::map<std::string, ScriptField>& GetFields() const { return m_Fields; }
    };

    /// <summary>
    /// 脚本实例：记录 C# 脚本实例的信息，调用实例方法
    /// </summary>
    class ScriptInstance
    {
    private:
        friend class ScriptEngine;
        friend struct ScriptFieldInstance;
    private:
        Ref<ScriptClass> m_ScriptClass;     // 脚本类

        MonoObject* m_Instance = nullptr;   // 对应的 Mono 实例

        MonoMethod* m_Constructor = nullptr;    // 构造方法
        MonoMethod* m_AwakeMethod = nullptr;    // Awake 方法
        MonoMethod* m_UpdateMethod = nullptr;   // Update 方法

        inline static char s_FieldValueBuffer[16];
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

        Ref<ScriptClass> GetScriptClass() { return m_ScriptClass; }

        template<typename T>
        T GetFieldValue(const std::string& name)
        {
            static_assert(sizeof(T) <= 16, "Type too large.");

            bool success = GetFieldValueInternal(name, s_FieldValueBuffer);
            if (!success)
            {
                return T();
            }
            return *(T*)s_FieldValueBuffer;
        }

        template<typename T>
        void SetFieldValue(const std::string& name, T value)
        {
            static_assert(sizeof(T) <= 16, "Type too large.");

            SetFieldValueInternal(name, &value);
        }
    private:
        /// <summary>
        /// 获取字段内部 (Mono) 值
        /// </summary>
        /// <param name="name">字段名</param>
        /// <param name="buffer">字段值缓冲区</param>
        /// <returns></returns>
        bool GetFieldValueInternal(const std::string& name, void* buffer);
        bool SetFieldValueInternal(const std::string& name, const void* value);
    };
}