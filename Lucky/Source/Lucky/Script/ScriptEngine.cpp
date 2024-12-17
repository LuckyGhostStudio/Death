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
        /// ��ȡ�ļ��ֽ� TODO move to FileSystem
        /// </summary>
        /// <param name="filepath">�ļ�·��</param>
        /// <param name="outSize">�����С</param>
        /// <returns></returns>
        static char* ReadBytes(const std::filesystem::path& filepath, uint32_t* outSize)
        {
            std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

            // ��ʧ��
            if (!stream)
            {
                return nullptr;
            }

            std::streampos end = stream.tellg();    // �ļ�ĩβλ��
            stream.seekg(0, std::ios::beg);         // ָ���ƶ����ļ���ͷ
            uint64_t size = end - stream.tellg();   // �ļ���С

            // �ļ�Ϊ��
            if (size == 0)
            {
                return nullptr;
            }

            char* buffer = new char[size];
            stream.read((char*)buffer, size);   // ��ȡ�ļ�
            stream.close();

            *outSize = (uint32_t)size;

            return buffer;
        }

        /// <summary>
        /// ���� Mono ����
        /// </summary>
        /// <param name="assemblyPath">����·��</param>
        /// <returns>Mono ����</returns>
        static MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath)
        {
            uint32_t fileSize = 0;
            char* fileData = ReadBytes(assemblyPath, &fileSize);    // ��ȡ�����ļ�

            MonoImageOpenStatus status;     // ���� Mono Image ʱ��״̬
            // ���� Mono Image��Image û�жԳ��򼯵�����
            MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

            // Mono Image ����ʧ��
            if (status != MONO_IMAGE_OK)
            {
                const char* errorMessage = mono_image_strerror(status); // ������Ϣ
                // TODO ��� errorMessage
                return nullptr;
            }

            // �� Mono Image ���س���
            std::string pathStr = assemblyPath.string();
            MonoAssembly* assembly = mono_assembly_load_from_full(image, pathStr.c_str(), &status, 0);

            mono_image_close(image);

            delete[] fileData;

            return assembly;
        }

        /// <summary>
        /// ��ӡ�����е�����
        /// </summary>
        /// <param name="assembly">Mono ����</param>
        static void PrintAssemblyTypes(MonoAssembly* assembly)
        {
            MonoImage* image = mono_assembly_get_image(assembly);   // Mono ���򼯵� Image
            const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);   // Mono ����� ��Ԫ����
            int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);                                  // ���е���������

            // ������������
            for (int32_t i = 0; i < numTypes; i++)
            {
                uint32_t cols[MONO_TYPEDEF_SIZE];   // ��������

                mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE); // ���� ����� ��ÿ��

                const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]); // ���͵������ռ���
                const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);           // ������

                LC_CORE_TRACE("{}.{}", nameSpace, name);
            }
        }

        static ScriptFieldType MonoTypeToScriptFieldType(MonoType* monoType)
        {
            std::string typeName = mono_type_get_name(monoType);    // ������

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
    /// �ű��������ݣ��洢 Mono ����
    /// </summary>
    struct ScriptEngineData
    {
        MonoDomain* RootDomain = nullptr;       // ���� ���� Mono �����Ļ��� ���غ��ĳ��� ��������Ӧ����
        MonoDomain* AppDomain = nullptr;        // Ӧ���� ���ڼ��غ������û������ C# �ű����� ���ж����ĳ��򼯺��������տռ�

        MonoAssembly* CoreAssembly = nullptr;   // ���ĳ���
        MonoImage* CoreAssemblyImage = nullptr; // ���ĳ��� Image

        MonoAssembly* AppAssembly = nullptr;    // App ����
        MonoImage* AppAssemblyImage = nullptr;  // App ���� Image

        ScriptClass MonoBehaviourClass;         // MonoBehaviour �� (C#)

        std::unordered_map<std::string, Ref<ScriptClass>> MonoBehaviourClasses; // namespace.class - MonoBehaviour ����
        std::unordered_map<UUID, Ref<ScriptInstance>> MonoBehaviourInstances;   // ObjectUUID - MonoBehaviour ����ʵ��

        std::unordered_map<UUID, ScriptFieldMap> MonoBehaviourScriptFields;     // ObjectUUID - MonoBehaviour �ű��ֶ�

        // Runtime TODO Ref
        Scene* SceneContext = nullptr;  // ����������
    };

    static ScriptEngineData* s_Data = nullptr;

    void ScriptEngine::Init()
    {
        s_Data = new ScriptEngineData();

        InitMono();
        LoadAssembly("Resources/Scripts/Lucky-ScriptCore.dll");     // ���غ��ĳ��򼯣��ű��ں�
        LoadAppAssembly("Project/Binaries/Assembly-CSharp.dll");    // ���� App ���򼯣���Ϸ�ű�
        LoadAssemblyClasses();                                      // ���س����е���

        auto& classes = s_Data->MonoBehaviourClasses;

        ScriptGlue::RegisterComponents();       // ע���������
        ScriptGlue::RegisterInternalCalls();    // ע���ڲ����ú���

        // �Ӻ��ĳ��򼯴��� LuckyEngine.MonoBehaviour ��
        s_Data->MonoBehaviourClass = ScriptClass("LuckyEngine", "MonoBehaviour", true);
    }

    void ScriptEngine::Shutdown()
    {
        ShutdownMono();
        delete s_Data;
    }

    void ScriptEngine::InitMono()
    {
        mono_set_assemblies_path("mono/lib");   // ���� Mono ����·��

        // ��ʼ�� Mono JIT ������ ��������
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
        // ���� App ��
        char domainName[] = "LuckyScriptRuntime";
        s_Data->AppDomain = mono_domain_create_appdomain(domainName, nullptr);
        mono_domain_set(s_Data->AppDomain, true);   // ���õ�ǰ�߳�ʹ�� �� AppDomain

        // ���� C# ���ĳ��� Lucky-ScriptCore.dll
        s_Data->CoreAssembly = Utils::LoadMonoAssembly(filepath);
        // ��ȡ ���ĳ��򼯵� Mono Image
        s_Data->CoreAssemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);

        // Utils::PrintAssemblyTypes(s_Data->CoreAssembly);
    }

    void ScriptEngine::LoadAppAssembly(const std::filesystem::path& filepath)
    {
        // ���� C# App ����
        s_Data->AppAssembly = Utils::LoadMonoAssembly(filepath);
        // ��ȡ ���򼯵� Mono Image
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

        s_Data->MonoBehaviourInstances.clear(); // ��� MonoBehaviour ʵ��
    }

    bool ScriptEngine::MonoBehaviourClassExists(const std::string& fullName)
    {
        return s_Data->MonoBehaviourClasses.find(fullName) != s_Data->MonoBehaviourClasses.end();
    }

    void ScriptEngine::OnCreateMonoBehaviour(Object object)
    {
        const ScriptComponent& scriptComponent = object.GetComponent<ScriptComponent>();    // Script ���
        std::string fullName = std::format("{}.{}", scriptComponent.ClassNamespace, scriptComponent.ClassName);
        // �ű���ȫ������
        if (ScriptEngine::MonoBehaviourClassExists(fullName))
        {
            UUID objectID = object.GetUUID();

            // �����ű�ʵ��
            Ref<ScriptInstance> instance = CreateRef<ScriptInstance>(s_Data->MonoBehaviourClasses[fullName], object);
            // ��ӽű�ʵ���� UUID - MonoBehaviour Map
            s_Data->MonoBehaviourInstances[objectID] = instance;

            // ���� ScriptFieldMap �е��ֶ�ֵ���ű�ʵ��
            if (s_Data->MonoBehaviourScriptFields.find(objectID) != s_Data->MonoBehaviourScriptFields.end())
            {
                const ScriptFieldMap& fieldMap = s_Data->MonoBehaviourScriptFields.at(objectID);
                for (const auto& [name, fieldInstance] : fieldMap)
                {
                    instance->SetFieldValueInternal(name, fieldInstance.m_ValueBuffer);
                }
            }

            instance->InvokeAwake();    // ���� Awake ����
        }
    }

    void ScriptEngine::OnUpdateMonoBehaviour(Object object, DeltaTime dt)
    {
        UUID objectUUID = object.GetUUID();
        LC_CORE_ASSERT(s_Data->MonoBehaviourInstances.find(objectUUID) != s_Data->MonoBehaviourInstances.end(), "MonoBehaviour Instance not found.");

        // ��ȡ�ű�ʵ��
        Ref<ScriptInstance> instance = s_Data->MonoBehaviourInstances[objectUUID];

        instance->InvokeUpdate((float)dt);  // ���� Update ����
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
        s_Data->MonoBehaviourClasses.clear();  // ��� MonoBehaviour �ű����б�
        // ��ȡ Mono ����� ��Ԫ����
        const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(s_Data->AppAssemblyImage, MONO_TABLE_TYPEDEF);
        int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);  // ���е���������

        // �Ӻ��ĳ��� Lucky-ScriptCore �м��� MonoBehaviour ��
        MonoClass* monoBehaviourClass = mono_class_from_name(s_Data->CoreAssemblyImage, "LuckyEngine", "MonoBehaviour");

        // ������������
        for (int32_t i = 0; i < numTypes; i++)
        {
            uint32_t cols[MONO_TYPEDEF_SIZE];   // ��������

            mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE); // ���� ����� ��ÿ��

            const char* nameSpace = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);  // ��������ռ���
            const char* className = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAME]);       // ����

            // ��ȫ��
            std::string fullName;
            if (strlen(nameSpace) != 0)
            {
                fullName = std::format("{}.{}", nameSpace, className);   // nameSpace.className
            }
            else
            {
                fullName = className;
            }

            // ��ǰ�� Mono ��
            MonoClass* monoClass = mono_class_from_name(s_Data->AppAssemblyImage, nameSpace, className);

            // ������������
            if (monoClass == monoBehaviourClass)
            {
                continue;
            }

            // ��� monoClass �Ƿ�Ϊ MonoBehaviour ������
            bool isMonoBehaviour = mono_class_is_subclass_of(monoClass, monoBehaviourClass, false);
            if (!isMonoBehaviour)
            {
                continue;
            }

            // ������ൽӳ���
            Ref<ScriptClass> scriptClass = CreateRef<ScriptClass>(nameSpace, className);
            s_Data->MonoBehaviourClasses[fullName] = scriptClass;

            LC_CORE_TRACE("{}.{}", nameSpace, className);

            // �ֶ�����
            int fieldCount = mono_class_num_fields(monoClass);
            LC_CORE_WARN("{} Fields {}:", className, fieldCount);

            // ���������ֶ�
            void* iterator = nullptr;
            while (MonoClassField* field = mono_class_get_fields(monoClass, &iterator))
            {
                const char* fieldName = mono_field_get_name(field); // �ֶ���
                uint32_t flags = mono_field_get_flags(field);       // �ֶεı�־

                // public �ֶ�
                if (flags & FIELD_ATTRIBUTE_PUBLIC)
                {
                    MonoType* type = mono_field_get_type(field);       // �ֶ�����
                    ScriptFieldType fieldType = Utils::MonoTypeToScriptFieldType(type);

                    LC_CORE_WARN("    public {0} {1}", Utils::ScriptFieldTypeToString(fieldType), fieldName);

                    scriptClass->m_Fields[fieldName] = { fieldType, fieldName, field }; // ����ֶε� map
                    // TODO ����ֶ�ֵ
                }
            }
        }
    }

    MonoObject* ScriptEngine::InstantiateClass(MonoClass* monoClass)
    {
        // ���� Mono �����
        MonoObject* instance = mono_object_new(s_Data->AppDomain, monoClass);
        // ���ù��캯����ʼ�� Mono ��ʵ��
        mono_runtime_object_init(instance);

        return instance;
    }

    ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore)
        : m_ClassNamespace(classNamespace), m_ClassName(className)
    {
        // �� Mono ���򼯵� Image ��ȡ classNamespace.className �ࣺ���ֺ��� �� App ���� Image
        m_MonoClass = mono_class_from_name(isCore ? s_Data->CoreAssemblyImage : s_Data->AppAssemblyImage, classNamespace.c_str(), className.c_str());
    }

    MonoObject* ScriptClass::Instantiate()
    {
        return ScriptEngine::InstantiateClass(m_MonoClass);
    }

    MonoMethod* ScriptClass::GetMethod(const std::string& name, int parameterCount)
    {
        // �� MonoClass �л�ȡ����
        return mono_class_get_method_from_name(m_MonoClass, name.c_str(), parameterCount);
    }

    MonoObject* ScriptClass::InvokeMethod(MonoObject* instance, MonoMethod* method, void** params)
    {
        // ���� MonoClass �� instance �ķ���
        return mono_runtime_invoke(method, instance, params, nullptr);
    }

    ScriptInstance::ScriptInstance(Ref<ScriptClass> scriptClass, Object object)
        : m_ScriptClass(scriptClass)
    {
        m_Instance = scriptClass->Instantiate();    // ���� Mono ��ʵ��

        // ��ȡ�෽��
        m_Constructor = s_Data->MonoBehaviourClass.GetMethod(".ctor", 1);   // MonoBehaviour ���캯��
        m_AwakeMethod = scriptClass->GetMethod("Awake", 0);
        m_UpdateMethod = scriptClass->GetMethod("Update", 1);

        // ���� MonoBehaviour ���캯�� MonoBehaviour(uuid)
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
            m_ScriptClass->InvokeMethod(m_Instance, m_AwakeMethod); // ���� Awake ����
        }
    }

    void ScriptInstance::InvokeUpdate(float dt)
    {
        if (m_UpdateMethod)
        {
            void* param = &dt;
            m_ScriptClass->InvokeMethod(m_Instance, m_UpdateMethod, &param);    // ���� Update ����
        }
    }

    bool ScriptInstance::GetFieldValueInternal(const std::string& name, void* buffer)
    {
        const auto& fields = m_ScriptClass->GetFields();

        if (fields.find(name) == fields.end())
        {
            return false;
        }

        const ScriptField& field = fields.at(name); // ��ǰ�ֶ�

        mono_field_get_value(m_Instance, field.ClassField, buffer); // ��ȡ�ֶ�ֵ

        return true;
    }

    bool ScriptInstance::SetFieldValueInternal(const std::string& name, const void* value)
    {
        const auto& fields = m_ScriptClass->GetFields();

        if (fields.find(name) == fields.end())
        {
            return false;
        }

        const ScriptField& field = fields.at(name); // ��ǰ�ֶ�

        mono_field_set_value(m_Instance, field.ClassField, (void*)value);  // �����ֶ�ֵ

        return true;
    }
}