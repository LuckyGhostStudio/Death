#include "lcpch.h"
#include "ScriptEngine.h"

#include "ScriptGlue.h"
#include "Lucky/Scene/Object.h"
#include "Lucky/Scene/Components/ScriptComponent.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"

namespace Lucky
{
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

        ScriptClass MonoBehaviourClass;         // MonoBehaviour �� (C#)

        std::unordered_map<std::string, Ref<ScriptClass>> MonoBehaviourClasses; // namespace.class - MonoBehaviour ����
        std::unordered_map<UUID, Ref<ScriptInstance>> MonoBehaviourInstances;   // ObjectUUID - MonoBehaviour ����ʵ��

        // Runtime TODO Ref
        Scene* SceneContext = nullptr;  // ����������
    };

    static ScriptEngineData* s_Data = nullptr;

    void ScriptEngine::Init()
    {
        s_Data = new ScriptEngineData();

        InitMono();
        LoadAssembly("Resources/Scripts/Lucky-ScriptCore.dll"); // ���غ��ĳ���
        LoadAssemblyClasses(s_Data->CoreAssembly);              // ���غ��ĳ�����

        auto& classes = s_Data->MonoBehaviourClasses;

        ScriptGlue::RegisterFunctions();    // ע���ڲ����ú���

        // ���� LuckyEngine.MonoBehaviour ��
        s_Data->MonoBehaviourClass = ScriptClass("LuckyEngine", "MonoBehaviour");
#if 0
        // - ���Դ� Mono ���� C# ��ͷ��� -
        
        MonoObject* instance = s_Data->MonoBehaviourClass.Instantiate();   // ʵ������

        // ��ȡ ���� printMessageFunc ����
        MonoMethod* printMessageFunc = s_Data->MonoBehaviourClass.GetMethod("PrintMessage", 0);
        s_Data->MonoBehaviourClass.InvokeMethod(instance, printMessageFunc, nullptr);

        // Main.PrintInt(int value)
        MonoMethod* printIntFunc = s_Data->MonoBehaviourClass.GetMethod("PrintInt", 1);
        int value = 5;
        void* param = &value;
        s_Data->MonoBehaviourClass.InvokeMethod(instance, printIntFunc, &param);

        // Main.PrintInts(int value1, int value2)
        MonoMethod* printIntsFunc = s_Data->MonoBehaviourClass.GetMethod( "PrintInts", 2);
        int value2 = 508;
        void* params[2] = { &value, &value2 };
        s_Data->MonoBehaviourClass.InvokeMethod(instance, printIntsFunc, params);

        // Main.PrintCustomMessage(string message)
        MonoString* monoString = mono_string_new(s_Data->AppDomain, "Hello LuckyEngine from C++.");   // String ��������
        MonoMethod* printCustomMessageFunc = s_Data->MonoBehaviourClass.GetMethod( "PrintCustomMessage", 1);
        void* stringParam = monoString;
        s_Data->MonoBehaviourClass.InvokeMethod(instance, printCustomMessageFunc, &stringParam);
#endif
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
            // �����ű�ʵ��
            Ref<ScriptInstance> instance = CreateRef<ScriptInstance>(s_Data->MonoBehaviourClasses[fullName], object);
            // ��ӽű�ʵ���� UUID - MonoBehaviour Map
            s_Data->MonoBehaviourInstances[object.GetUUID()] = instance;

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

    std::unordered_map<std::string, Ref<ScriptClass>> ScriptEngine::GetMonoBehaviourClasses()
    {
        return s_Data->MonoBehaviourClasses;
    }

    void ScriptEngine::LoadAssemblyClasses(MonoAssembly* assembly)
    {
        s_Data->MonoBehaviourClasses.clear();  // ��� MonoBehaviour �ű����б�

        MonoImage* image = mono_assembly_get_image(assembly);   // Mono ���򼯵� Image
        const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);   // Mono ����� ��Ԫ����
        int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);                                  // ���е���������

        // MonoBehaviour ��
        MonoClass* monoBehaviourClass = mono_class_from_name(s_Data->CoreAssemblyImage, "LuckyEngine", "MonoBehaviour");

        // ������������
        for (int32_t i = 0; i < numTypes; i++)
        {
            uint32_t cols[MONO_TYPEDEF_SIZE];   // ��������

            mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE); // ���� ����� ��ÿ��

            const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]); // ���͵������ռ���
            const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);           // ������

            // ��ȫ��
            std::string fullName;
            if (strlen(nameSpace) != 0)
            {
                fullName = std::format("{}.{}", nameSpace, name);   // nameSpace.name
            }
            else
            {
                fullName = name;
            }

            // Mono ��
            MonoClass* monoClass = mono_class_from_name(s_Data->CoreAssemblyImage, nameSpace, name);

            // ������������
            if (monoClass == monoBehaviourClass)
            {
                continue;
            }

            // ��� monoClass �Ƿ�Ϊ MonoBehaviour ������
            bool isMonoBehaviour = mono_class_is_subclass_of(monoClass, monoBehaviourClass, false);
            if (isMonoBehaviour)
            {
                s_Data->MonoBehaviourClasses[fullName] = CreateRef<ScriptClass>(nameSpace, name);  // ��ӵ�ӳ���
            }

            LC_CORE_TRACE("{}.{}", nameSpace, name);
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

    ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className)
        : m_ClassNamespace(classNamespace), m_ClassName(className)
    {
        // �� Mono ���򼯵� Image ��ȡ classNamespace.className ��
        m_MonoClass = mono_class_from_name(s_Data->CoreAssemblyImage, classNamespace.c_str(), className.c_str());
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

        // ���� GameObject ���캯�� GameObejct(uuid)
        {
            UUID uuid = object.GetUUID();
            void* param = &uuid;
            m_ScriptClass->InvokeMethod(m_Instance, m_Constructor, &param);
        }
    }

    void ScriptInstance::InvokeAwake()
    {
        m_ScriptClass->InvokeMethod(m_Instance, m_AwakeMethod); // ���� Awake ����
    }

    void ScriptInstance::InvokeUpdate(float dt)
    {
        void* param = &dt;
        m_ScriptClass->InvokeMethod(m_Instance, m_UpdateMethod, &param);    // ���� Update ����
    }
}