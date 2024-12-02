#include "lcpch.h"
#include "ScriptEngine.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"

namespace Lucky
{
    /// <summary>
    /// 脚本引擎数据：存储 Mono 数据
    /// </summary>
    struct ScriptEngineData
    {
        MonoDomain* RootDomain = nullptr;       // 根域 管理 Mono 环境的基础 加载核心程序集 管理所有应用域
        MonoDomain* AppDomain = nullptr;        // 应用域 用于加载和运行用户定义的 C# 脚本程序集 具有独立的程序集和垃圾回收空间

        MonoAssembly* CoreAssembly = nullptr;   // 核心程序集
    };

    static ScriptEngineData* s_Data = nullptr;

    void ScriptEngine::Init()
    {
        s_Data = new ScriptEngineData();

        InitMono();
    }

    void ScriptEngine::Shutdown()
    {
        ShutdownMono();
        delete s_Data;
    }

    /// <summary>
    /// 读取文件字节
    /// </summary>
    /// <param name="filepath">文件路径</param>
    /// <param name="outSize">输出大小</param>
    /// <returns></returns>
    char* ReadBytes(const std::string& filepath, uint32_t* outSize)
    {
        std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

        // 打开失败
        if (!stream)
        {
            return nullptr;
        }

        std::streampos end = stream.tellg();    // 文件末尾位置
        stream.seekg(0, std::ios::beg);         // 指针移动到文件开头
        uint32_t size = end - stream.tellg();   // 文件大小

        // 文件为空
        if (size == 0)
        {
            return nullptr;
        }

        char* buffer = new char[size];
        stream.read((char*)buffer, size);   // 读取文件
        stream.close();

        *outSize = size;

        return buffer;
    }

    /// <summary>
    /// 加载 C# 程序集
    /// </summary>
    /// <param name="assemblyPath">程序集路径</param>
    /// <returns>Mono 程序集</returns>
    MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath)
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
        MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);

        mono_image_close(image);

        delete[] fileData;

        return assembly;
    }

    /// <summary>
    /// 打印程序集中的类型
    /// </summary>
    /// <param name="assembly">Mono 程序集</param>
    void PrintAssemblyTypes(MonoAssembly* assembly)
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

    void ScriptEngine::InitMono()
    {
        mono_set_assemblies_path("mono/lib");   // 设置 Mono 程序集路径

        // 初始化 Mono JIT 编译器 创建根域
        MonoDomain* rootDomain = mono_jit_init("LuckyJITRuntime");
        LC_CORE_ASSERT(rootDomain, "rootDomain is null.");

        s_Data->RootDomain = rootDomain;

        // 创建 App 域
        char domainName[] = "LuckyScriptRuntime";
        s_Data->AppDomain = mono_domain_create_appdomain(domainName, nullptr);
        mono_domain_set(s_Data->AppDomain, true);   // 设置当前线程使用 该 AppDomain

        // 加载 C# 核心程序集 Lucky-ScriptCore.dll
        s_Data->CoreAssembly = LoadCSharpAssembly("Resources/Scripts/Lucky-ScriptCore.dll");
        PrintAssemblyTypes(s_Data->CoreAssembly);

        MonoImage* assemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);       // 获取 核心程序集的 Mono Image
        
        // - 测试从 Mono 调用 C# 类和方法
        // 从 Mono 程序集的 Image 获取 LuckyEngine.Main 类
        MonoClass* monoClass = mono_class_from_name(assemblyImage, "LuckyEngine", "Main");
        // 创建 Main 类对象
        MonoObject* instance = mono_object_new(s_Data->AppDomain, monoClass);
        // 调用构造函数初始化 Main 类实例
        mono_runtime_object_init(instance);

        // 获取函数 Main.PrintMessage()
        MonoMethod* printMessageFunc = mono_class_get_method_from_name(monoClass, "PrintMessage", 0);
        // 调用函数
        mono_runtime_invoke(printMessageFunc, instance, nullptr, nullptr);

        // Main.PrintInt(int value)
        MonoMethod* printIntFunc = mono_class_get_method_from_name(monoClass, "PrintInt", 1);

        int value = 5;
        void* param = &value;

        mono_runtime_invoke(printIntFunc, instance, &param, nullptr);

        // Main.PrintInts(int value1, int value2)
        MonoMethod* printIntsFunc = mono_class_get_method_from_name(monoClass, "PrintInts", 2);
        int value2 = 508;
        void* params[2] = { &value, &value2 };
        mono_runtime_invoke(printIntsFunc, instance, params, nullptr);

        // Main.PrintCustomMessage(string message)
        MonoString* monoString = mono_string_new(s_Data->AppDomain, "Hello LuckyEngine from C++.");   // String 类型数据
        MonoMethod* printCustomMessageFunc = mono_class_get_method_from_name(monoClass, "PrintCustomMessage", 1);
        void* stringParam = monoString;
        mono_runtime_invoke(printCustomMessageFunc, instance, &stringParam, nullptr);
    }

    void ScriptEngine::ShutdownMono()
    {
        // TODO Other

        // mono_domain_unload(s_Data->AppDomain);
        s_Data->AppDomain = nullptr;

        // mono_jit_cleanup(s_Data->RootDomain);
        s_Data->RootDomain = nullptr;
    }

}