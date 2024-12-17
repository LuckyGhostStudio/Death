#include "lcpch.h"
#include "ScriptEngine.h"

#include "ScriptGlue.h"
#include "Lucky/Scene/Object.h"
#include "Lucky/Scene/Components/ScriptComponent.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
#include "mono/metadata/tabledefs.h"

namespace Lucky
{
    // MonoTypeName - ScriptFieldType
    static std::unordered_map<std::string, ScriptFieldType> s_ScriptFieldTypeMap =
    {
        { "System.Single",          ScriptFieldType::Float      },
        { "System.Double",          ScriptFieldType::Double     },
        { "System.Boolean",         ScriptFieldType::Bool       },
        { "System.SByte",           ScriptFieldType::SByte      },
        { "System.Int16",           ScriptFieldType::Short      },
        { "System.Int32",           ScriptFieldType::Int        },
        { "System.Int64",           ScriptFieldType::Long       },
        { "System.Byte",            ScriptFieldType::Byte       },
        { "System.UInt16",          ScriptFieldType::UShort     },
        { "System.UInt32",          ScriptFieldType::UInt       },
        { "System.UInt64",          ScriptFieldType::ULong      },
        { "System.Char",            ScriptFieldType::Char       },
        { "System.String",          ScriptFieldType::String     },
        { "LuckyEngine.Vector2",    ScriptFieldType::Vector2    },
        { "LuckyEngine.Vector3",    ScriptFieldType::Vector3    },
        { "LuckyEngine.Vector4",    ScriptFieldType::Vector4    },
        { "LuckyEngine.GameObject", ScriptFieldType::GameObject }
    };

    namespace Utils
    {
        /// <summary>
        /// 读取文件字节 TODO move to FileSystem
        /// </summary>
        /// <param name="filepath">文件路径</param>
        /// <param name="outSize">输出大小</param>
        /// <returns></returns>
        static char* ReadBytes(const std::filesystem::path& filepath, uint32_t* outSize)
        {
            std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

            // 打开失败
            if (!stream)
            {
                return nullptr;
            }

            std::streampos end = stream.tellg();    // 文件末尾位置
            stream.seekg(0, std::ios::beg);         // 指针移动到文件开头
            uint64_t size = end - stream.tellg();   // 文件大小

            // 文件为空
            if (size == 0)
            {
                return nullptr;
            }

            char* buffer = new char[size];
            stream.read((char*)buffer, size);   // 读取文件
            stream.close();

            *outSize = (uint32_t)size;

            return buffer;
        }

        /// <summary>
        /// 加载 Mono 程序集
        /// </summary>
        /// <param name="assemblyPath">程序集路径</param>
        /// <returns>Mono 程序集</returns>
        static MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath)
        {
            uint32_t fileSize = 0;
            char* fileData = ReadBytes(assemblyPath, &fileSize);    // 读取程序集文件

            MonoImageOpenStatus status;     // 加载 Mono Image 时的状态
            // 创建 Mono Image：Image 没有对程序集的引用
            MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

            // Mono Image 加载失败
            if (status != MONO_IMAGE_OK)
            {
                const char* errorMessage = mono_image_strerror(status); // 错误消息
                // TODO 输出 errorMessage
                return nullptr;
            }

            // 从 Mono Image 加载程序集
            std::string pathStr = assemblyPath.string();
            MonoAssembly* assembly = mono_assembly_load_from_full(image, pathStr.c_str(), &status, 0);

            mono_image_close(image);

            delete[] fileData;

            return assembly;
        }

        /// <summary>
        /// 打印程序集中的类型
        /// </summary>
        /// <param name="assembly">Mono 程序集</param>
        static void PrintAssemblyTypes(MonoAssembly* assembly)
        {
            MonoImage* image = mono_assembly_get_image(assembly);   // Mono 程序集的 Image
            const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);   // Mono 定义表 的元数据
            int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);                                  // 表中的类型数量

            // 遍历所有类型
            for (int32_t i = 0; i < numTypes; i++)
            {
                uint32_t cols[MONO_TYPEDEF_SIZE];   // 单行数据

                mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE); // 解码 定义表 的每行

                const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]); // 类型的命名空间名
                const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);           // 类型名

                LC_CORE_TRACE("{}.{}", nameSpace, name);
            }
        }

        static ScriptFieldType MonoTypeToScriptFieldType(MonoType* monoType)
        {
            std::string typeName = mono_type_get_name(monoType);    // 类型名

            if (s_ScriptFieldTypeMap.find(typeName) == s_ScriptFieldTypeMap.end())
            {
                return ScriptFieldType::None;
            }
            return s_ScriptFieldTypeMap.at(typeName);
        }

        static const char* ScriptFieldTypeToString(ScriptFieldType fieldType)
        {
            switch (fieldType)
            {
                case ScriptFieldType::Float:        return "Float";
                case ScriptFieldType::Double:       return "Double";
                case ScriptFieldType::Bool:         return "Bool";
                case ScriptFieldType::SByte:        return "SByte";
                case ScriptFieldType::Short:        return "Short";
                case ScriptFieldType::Int:          return "Int";
                case ScriptFieldType::Long:         return "Long";
                case ScriptFieldType::Byte:         return "Byte";
                case ScriptFieldType::UShort:       return "UShort";
                case ScriptFieldType::UInt:         return "UInt";
                case ScriptFieldType::ULong:        return "ULong";
                case ScriptFieldType::Char:         return "Char";
                case ScriptFieldType::String:       return "String";
                case ScriptFieldType::Vector2:      return "Vector2";
                case ScriptFieldType::Vector3:      return "Vector3";
                case ScriptFieldType::Vector4:      return "Vector4";
                case ScriptFieldType::GameObject:   return "GameObject";
            }
            return "<Invalid>";
        }
    }

    /// <summary>
    /// 脚本引擎数据：存储 Mono 数据
    /// </summary>
    struct ScriptEngineData
    {
        MonoDomain* RootDomain = nullptr;       // 根域 管理 Mono 环境的基础 加载核心程序集 管理所有应用域
        MonoDomain* AppDomain = nullptr;        // 应用域 用于加载和运行用户定义的 C# 脚本程序集 具有独立的程序集和垃圾回收空间

        MonoAssembly* CoreAssembly = nullptr;   // 核心程序集
        MonoImage* CoreAssemblyImage = nullptr; // 核心程序集 Image

        MonoAssembly* AppAssembly = nullptr;    // App 程序集
        MonoImage* AppAssemblyImage = nullptr;  // App 程序集 Image

        ScriptClass MonoBehaviourClass;         // MonoBehaviour 类 (C#)

        std::unordered_map<std::string, Ref<ScriptClass>> MonoBehaviourClasses; // namespace.class - MonoBehaviour 子类
        std::unordered_map<UUID, Ref<ScriptInstance>> MonoBehaviourInstances;   // ObjectUUID - MonoBehaviour 子类实例

        std::unordered_map<UUID, ScriptFieldMap> MonoBehaviourScriptFields;     // ObjectUUID - MonoBehaviour 脚本字段

        // Runtime TODO Ref
        Scene* SceneContext = nullptr;  // 场景上下文
    };

    static ScriptEngineData* s_Data = nullptr;

    void ScriptEngine::Init()
    {
        s_Data = new ScriptEngineData();

        InitMono();
        LoadAssembly("Resources/Scripts/Lucky-ScriptCore.dll");     // 加载核心程序集：脚本内核
        LoadAppAssembly("Project/Binaries/Assembly-CSharp.dll");    // 加载 App 程序集：游戏脚本
        LoadAssemblyClasses();                                      // 加载程序集中的类

        auto& classes = s_Data->MonoBehaviourClasses;

        ScriptGlue::RegisterComponents();       // 注册所有组件
        ScriptGlue::RegisterInternalCalls();    // 注册内部调用函数

        // 从核心程序集创建 LuckyEngine.MonoBehaviour 类
        s_Data->MonoBehaviourClass = ScriptClass("LuckyEngine", "MonoBehaviour", true);
    }

    void ScriptEngine::Shutdown()
    {
        ShutdownMono();
        delete s_Data;
    }

    void ScriptEngine::InitMono()
    {
        mono_set_assemblies_path("mono/lib");   // 设置 Mono 程序集路径

        // 初始化 Mono JIT 编译器 创建根域
        MonoDomain* rootDomain = mono_jit_init("LuckyJITRuntime");
        LC_CORE_ASSERT(rootDomain, "rootDomain is null.");

        s_Data->RootDomain = rootDomain;
    }

    void ScriptEngine::ShutdownMono()
    {
        // TODO Other

        // mono_domain_unload(s_Data->AppDomain);
        s_Data->AppDomain = nullptr;

        // mono_jit_cleanup(s_Data->RootDomain);
        s_Data->RootDomain = nullptr;
    }

    void ScriptEngine::LoadAssembly(const std::filesystem::path& filepath)
    {
        // 创建 App 域
        char domainName[] = "LuckyScriptRuntime";
        s_Data->AppDomain = mono_domain_create_appdomain(domainName, nullptr);
        mono_domain_set(s_Data->AppDomain, true);   // 设置当前线程使用 该 AppDomain

        // 加载 C# 核心程序集 Lucky-ScriptCore.dll
        s_Data->CoreAssembly = Utils::LoadMonoAssembly(filepath);
        // 获取 核心程序集的 Mono Image
        s_Data->CoreAssemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);

        // Utils::PrintAssemblyTypes(s_Data->CoreAssembly);
    }

    void ScriptEngine::LoadAppAssembly(const std::filesystem::path& filepath)
    {
        // 加载 C# App 程序集
        s_Data->AppAssembly = Utils::LoadMonoAssembly(filepath);
        // 获取 程序集的 Mono Image
        s_Data->AppAssemblyImage = mono_assembly_get_image(s_Data->AppAssembly);

        // Utils::PrintAssemblyTypes(s_Data->AppAssembly);
    }

    void ScriptEngine::OnRuntimeStart(Scene* scene)
    {
        s_Data->SceneContext = scene;
    }

    void ScriptEngine::OnRuntimeStop()
    {
        s_Data->SceneContext = nullptr;

        s_Data->MonoBehaviourInstances.clear(); // 清空 MonoBehaviour 实例
    }

    bool ScriptEngine::MonoBehaviourClassExists(const std::string& fullName)
    {
        return s_Data->MonoBehaviourClasses.find(fullName) != s_Data->MonoBehaviourClasses.end();
    }

    void ScriptEngine::OnCreateMonoBehaviour(Object object)
    {
        const ScriptComponent& scriptComponent = object.GetComponent<ScriptComponent>();    // Script 组件
        std::string fullName = std::format("{}.{}", scriptComponent.ClassNamespace, scriptComponent.ClassName);
        // 脚本类全名存在
        if (ScriptEngine::MonoBehaviourClassExists(fullName))
        {
            UUID objectID = object.GetUUID();

            // 创建脚本实例
            Ref<ScriptInstance> instance = CreateRef<ScriptInstance>(s_Data->MonoBehaviourClasses[fullName], object);
            // 添加脚本实例到 UUID - MonoBehaviour Map
            s_Data->MonoBehaviourInstances[objectID] = instance;

            // 复制 ScriptFieldMap 中的字段值到脚本实例
            if (s_Data->MonoBehaviourScriptFields.find(objectID) != s_Data->MonoBehaviourScriptFields.end())
            {
                const ScriptFieldMap& fieldMap = s_Data->MonoBehaviourScriptFields.at(objectID);
                for (const auto& [name, fieldInstance] : fieldMap)
                {
                    instance->SetFieldValueInternal(name, fieldInstance.m_ValueBuffer);
                }
            }

            instance->InvokeAwake();    // 调用 Awake 方法
        }
    }

    void ScriptEngine::OnUpdateMonoBehaviour(Object object, DeltaTime dt)
    {
        UUID objectUUID = object.GetUUID();
        LC_CORE_ASSERT(s_Data->MonoBehaviourInstances.find(objectUUID) != s_Data->MonoBehaviourInstances.end(), "MonoBehaviour Instance not found.");

        // 获取脚本实例
        Ref<ScriptInstance> instance = s_Data->MonoBehaviourInstances[objectUUID];

        instance->InvokeUpdate((float)dt);  // 调用 Update 方法
    }

    Scene* ScriptEngine::GetSceneContext()
    {
        return s_Data->SceneContext;
    }

    Ref<ScriptInstance> ScriptEngine::GetMonoBehaviourScriptInstance(UUID objectID)
    {
        if (s_Data->MonoBehaviourInstances.find(objectID) == s_Data->MonoBehaviourInstances.end())
        {
            return nullptr;
        }
        return s_Data->MonoBehaviourInstances.at(objectID);
    }

    Ref<ScriptClass> ScriptEngine::GetMonoBehaviourClass(const std::string& name)
    {
        if (s_Data->MonoBehaviourClasses.find(name) == s_Data->MonoBehaviourClasses.end())
        {
            return nullptr;
        }
        return s_Data->MonoBehaviourClasses.at(name);
    }

    std::unordered_map<std::string, Ref<ScriptClass>> ScriptEngine::GetMonoBehaviourClasses()
    {
        return s_Data->MonoBehaviourClasses;
    }

    ScriptFieldMap& ScriptEngine::GetScriptFieldMap(UUID objectID)
    {
        return s_Data->MonoBehaviourScriptFields[objectID];
    }

    MonoImage* ScriptEngine::GetCoreAssemblyImage()
    {
        return s_Data->CoreAssemblyImage;
    }

    void ScriptEngine::LoadAssemblyClasses()
    {
        s_Data->MonoBehaviourClasses.clear();  // 清空 MonoBehaviour 脚本类列表
        // 获取 Mono 定义表 的元数据
        const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(s_Data->AppAssemblyImage, MONO_TABLE_TYPEDEF);
        int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);  // 表中的类型数量

        // 从核心程序集 Lucky-ScriptCore 中加载 MonoBehaviour 类
        MonoClass* monoBehaviourClass = mono_class_from_name(s_Data->CoreAssemblyImage, "LuckyEngine", "MonoBehaviour");

        // 遍历所有类型
        for (int32_t i = 0; i < numTypes; i++)
        {
            uint32_t cols[MONO_TYPEDEF_SIZE];   // 单行数据

            mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE); // 解码 定义表 的每行

            const char* nameSpace = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);  // 类的命名空间名
            const char* className = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAME]);       // 类名

            // 类全名
            std::string fullName;
            if (strlen(nameSpace) != 0)
            {
                fullName = std::format("{}.{}", nameSpace, className);   // nameSpace.className
            }
            else
            {
                fullName = className;
            }

            // 当前的 Mono 类
            MonoClass* monoClass = mono_class_from_name(s_Data->AppAssemblyImage, nameSpace, className);

            // 跳过父类自身
            if (monoClass == monoBehaviourClass)
            {
                continue;
            }

            // 检查 monoClass 是否为 MonoBehaviour 的子类
            bool isMonoBehaviour = mono_class_is_subclass_of(monoClass, monoBehaviourClass, false);
            if (!isMonoBehaviour)
            {
                continue;
            }

            // 添加子类到映射表
            Ref<ScriptClass> scriptClass = CreateRef<ScriptClass>(nameSpace, className);
            s_Data->MonoBehaviourClasses[fullName] = scriptClass;

            LC_CORE_TRACE("{}.{}", nameSpace, className);

            // 字段数量
            int fieldCount = mono_class_num_fields(monoClass);
            LC_CORE_WARN("{} Fields {}:", className, fieldCount);

            // 迭代所有字段
            void* iterator = nullptr;
            while (MonoClassField* field = mono_class_get_fields(monoClass, &iterator))
            {
                const char* fieldName = mono_field_get_name(field); // 字段名
                uint32_t flags = mono_field_get_flags(field);       // 字段的标志

                // public 字段
                if (flags & FIELD_ATTRIBUTE_PUBLIC)
                {
                    MonoType* type = mono_field_get_type(field);       // 字段类型
                    ScriptFieldType fieldType = Utils::MonoTypeToScriptFieldType(type);

                    LC_CORE_WARN("    public {0} {1}", Utils::ScriptFieldTypeToString(fieldType), fieldName);

                    scriptClass->m_Fields[fieldName] = { fieldType, fieldName, field }; // 添加字段到 map
                    // TODO 添加字段值
                }
            }
        }
    }

    MonoObject* ScriptEngine::InstantiateClass(MonoClass* monoClass)
    {
        // 创建 Mono 类对象
        MonoObject* instance = mono_object_new(s_Data->AppDomain, monoClass);
        // 调用构造函数初始化 Mono 类实例
        mono_runtime_object_init(instance);

        return instance;
    }

    ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore)
        : m_ClassNamespace(classNamespace), m_ClassName(className)
    {
        // 从 Mono 程序集的 Image 获取 classNamespace.className 类：区分核心 或 App 程序集 Image
        m_MonoClass = mono_class_from_name(isCore ? s_Data->CoreAssemblyImage : s_Data->AppAssemblyImage, classNamespace.c_str(), className.c_str());
    }

    MonoObject* ScriptClass::Instantiate()
    {
        return ScriptEngine::InstantiateClass(m_MonoClass);
    }

    MonoMethod* ScriptClass::GetMethod(const std::string& name, int parameterCount)
    {
        // 从 MonoClass 中获取方法
        return mono_class_get_method_from_name(m_MonoClass, name.c_str(), parameterCount);
    }

    MonoObject* ScriptClass::InvokeMethod(MonoObject* instance, MonoMethod* method, void** params)
    {
        // 调用 MonoClass 的 instance 的方法
        return mono_runtime_invoke(method, instance, params, nullptr);
    }

    ScriptInstance::ScriptInstance(Ref<ScriptClass> scriptClass, Object object)
        : m_ScriptClass(scriptClass)
    {
        m_Instance = scriptClass->Instantiate();    // 创建 Mono 类实例

        // 获取类方法
        m_Constructor = s_Data->MonoBehaviourClass.GetMethod(".ctor", 1);   // MonoBehaviour 构造函数
        m_AwakeMethod = scriptClass->GetMethod("Awake", 0);
        m_UpdateMethod = scriptClass->GetMethod("Update", 1);

        // 调用 MonoBehaviour 构造函数 MonoBehaviour(uuid)
        {
            UUID uuid = object.GetUUID();
            void* param = &uuid;
            m_ScriptClass->InvokeMethod(m_Instance, m_Constructor, &param);
        }
    }

    void ScriptInstance::InvokeAwake()
    {
        if (m_AwakeMethod)
        {
            m_ScriptClass->InvokeMethod(m_Instance, m_AwakeMethod); // 调用 Awake 方法
        }
    }

    void ScriptInstance::InvokeUpdate(float dt)
    {
        if (m_UpdateMethod)
        {
            void* param = &dt;
            m_ScriptClass->InvokeMethod(m_Instance, m_UpdateMethod, &param);    // 调用 Update 方法
        }
    }

    bool ScriptInstance::GetFieldValueInternal(const std::string& name, void* buffer)
    {
        const auto& fields = m_ScriptClass->GetFields();

        if (fields.find(name) == fields.end())
        {
            return false;
        }

        const ScriptField& field = fields.at(name); // 当前字段

        mono_field_get_value(m_Instance, field.ClassField, buffer); // 获取字段值

        return true;
    }

    bool ScriptInstance::SetFieldValueInternal(const std::string& name, const void* value)
    {
        const auto& fields = m_ScriptClass->GetFields();

        if (fields.find(name) == fields.end())
        {
            return false;
        }

        const ScriptField& field = fields.at(name); // 当前字段

        mono_field_set_value(m_Instance, field.ClassField, (void*)value);  // 设置字段值

        return true;
    }
}