#include "lcpch.h"
#include "ScriptEngine.h"

#include "ScriptGlue.h"

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

        ScriptClass GameObjectClass;    // GameObject �� (C#)
    };

    static ScriptEngineData* s_Data = nullptr;

    void ScriptEngine::Init()
    {
        s_Data = new ScriptEngineData();

        InitMono();
        LoadAssembly("Resources/Scripts/Lucky-ScriptCore.dll"); // ���غ��ĳ���

        ScriptGlue::RegisterFunctions();    // ע���ڲ����ú���

        // - ���Դ� Mono ���� C# ��ͷ��� -
        // ���� LuckyEngine.GameObject ��
        s_Data->GameObjectClass = ScriptClass("LuckyEngine", "GameObject");
        
        MonoObject* instance = s_Data->GameObjectClass.Instantiate();   // ʵ������

        // ��ȡ ���� printMessageFunc ����
        MonoMethod* printMessageFunc = s_Data->GameObjectClass.GetMethod("PrintMessage", 0);
        s_Data->GameObjectClass.InvokeMethod(instance, printMessageFunc, nullptr);

        // Main.PrintInt(int value)
        MonoMethod* printIntFunc = s_Data->GameObjectClass.GetMethod("PrintInt", 1);
        int value = 5;
        void* param = &value;
        s_Data->GameObjectClass.InvokeMethod(instance, printIntFunc, &param);

        // Main.PrintInts(int value1, int value2)
        MonoMethod* printIntsFunc = s_Data->GameObjectClass.GetMethod( "PrintInts", 2);
        int value2 = 508;
        void* params[2] = { &value, &value2 };
        s_Data->GameObjectClass.InvokeMethod(instance, printIntsFunc, params);

        // Main.PrintCustomMessage(string message)
        MonoString* monoString = mono_string_new(s_Data->AppDomain, "Hello LuckyEngine from C++.");   // String ��������
        MonoMethod* printCustomMessageFunc = s_Data->GameObjectClass.GetMethod( "PrintCustomMessage", 1);
        void* stringParam = monoString;
        s_Data->GameObjectClass.InvokeMethod(instance, printCustomMessageFunc, &stringParam);
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
}