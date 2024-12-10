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
    /// �ű����棺���� CSharp Mono 
    /// </summary>
    class ScriptEngine
    {
    private:
        friend class ScriptClass;
        friend class ScriptGlue;
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

        static void LoadAppAssembly(const std::filesystem::path& filepath);

        static void OnRuntimeStart(Scene* scene);
        static void OnRuntimeStop();

        /// <summary>
        /// MonoBehaviour ����ű��Ƿ����
        /// </summary>
        /// <param name="fullName">namespace.class</param>
        /// <returns></returns>
        static bool MonoBehaviourClassExists(const std::string& fullName);

        /// <summary>
        /// ���� MonoBehaviour ����ű�ʵ��
        /// </summary>
        /// <param name="object"></param>
        static void OnCreateMonoBehaviour(Object object);

        /// <summary>
        /// ���� MonoBehaviour
        /// </summary>
        /// <param name="object">��ǰ����</param>
        /// <param name="dt">֡���</param>
        static void OnUpdateMonoBehaviour(Object object, DeltaTime dt);

        static Scene* GetSceneContext();
        static std::unordered_map<std::string, Ref<ScriptClass>> GetMonoBehaviourClasses();

        static MonoImage* GetCoreAssemblyImage();
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

        /// <summary>
        /// ���س����е���
        /// </summary>
        static void LoadAssemblyClasses();
    };

    /// <summary>
    /// �ű��ࣺ��¼ C# �ű�����Ϣ��ʵ���� �� �����෽�� TODO Temp move to other
    /// </summary>
    class ScriptClass
    {
    private:
        std::string m_ClassNamespace;   // �����ռ���
        std::string m_ClassName;        // ����

        MonoClass* m_MonoClass = nullptr;   // ��Ӧ�� Mono ��
    public:
        ScriptClass() = default;
        ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore = false);

        /// <summary>
        /// ���� Mono ��ʵ��
        /// </summary>
        /// <returns>ʵ��</returns>
        MonoObject* Instantiate();

        /// <summary>
        /// ��ȡ����
        /// </summary>
        /// <param name="name">������</param>
        /// <param name="parameterCount">��������</param>
        /// <returns></returns>
        MonoMethod* GetMethod(const std::string& name, int parameterCount);

        /// <summary>
        /// ���÷���
        /// </summary>
        /// <param name="instance">����ʵ��</param>
        /// <param name="method">����</param>
        /// <param name="params">�����б�</param>
        /// <returns></returns>
        MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);
    };

    /// <summary>
    /// �ű�ʵ������¼ C# �ű�ʵ������Ϣ������ʵ������
    /// </summary>
    class ScriptInstance
    {
    private:
        Ref<ScriptClass> m_ScriptClass;     // �ű���

        MonoObject* m_Instance = nullptr;   // ��Ӧ�� Mono ʵ��

        MonoMethod* m_Constructor = nullptr;    // ���췽��
        MonoMethod* m_AwakeMethod = nullptr;    // Awake ����
        MonoMethod* m_UpdateMethod = nullptr;   // Update ����
    public:
        ScriptInstance(Ref<ScriptClass> scriptClass, Object object);

        /// <summary>
        /// ���� Awake ����
        /// </summary>
        void InvokeAwake();

        /// <summary>
        /// ���� Update ����
        /// </summary>
        /// <param name="dt"></param>
        void InvokeUpdate(float dt);
    };
}