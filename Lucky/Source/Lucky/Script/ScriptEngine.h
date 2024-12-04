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
    /// �ű����棺���� CSharp Mono 
    /// </summary>
    class ScriptEngine
    {
    private:
        friend class ScriptClass;
    public:
        /// <summary>
        /// ��ʼ��
        /// </summary>
        static void Init();

        /// <summary>
        /// ����
        /// </summary>
        static void Shutdown();

        /// <summary>
        /// ���غ��ĳ���
        /// </summary>
        /// <param name="filepath">����·��</param>
        static void LoadAssembly(const std::filesystem::path& filepath);
    private:
        /// <summary>
        /// ��ʼ�� Mono ����ʱ
        /// </summary>
        static void InitMono();

        /// <summary>
        /// ���� Mono ����ʱ
        /// </summary>
        static void ShutdownMono();

        /// <summary>
        /// ʵ���� Mono �����
        /// </summary>
        /// <param name="monoClass">����</param>
        /// <returns>�����</returns>
        static MonoObject* InstantiateClass(MonoClass* monoClass);
    };

    /// <summary>
    /// �ű��ࣺ���ڴ�����ʵ���� C# �࣬�����෽�� TODO Temp move to other
    /// </summary>
    class ScriptClass
    {
    private:
        std::string m_ClassNamespace;   // �����ռ���
        std::string m_ClassName;        // ����

        MonoClass* m_MonoClass = nullptr;   // ��Ӧ�� Mono ��
    public:
        ScriptClass() = default;
        ScriptClass(const std::string& classNamespace, const std::string& className);

        MonoObject* Instantiate();
        MonoMethod* GetMethod(const std::string& name, int parameterCount);
        MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);
    };
}