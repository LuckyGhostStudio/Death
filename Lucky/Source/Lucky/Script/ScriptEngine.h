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
    /// �ű��ֶ�
    /// </summary>
    struct ScriptField
    {
        ScriptFieldType Type;   // ����
        std::string Name;       // ����

        MonoClassField* ClassField;
    };

    /// <summary>
    /// �ű��ֶ�ʵ�� ScriptField + Data
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
        uint8_t m_ValueBuffer[16];  // �ֶ�ֵ Buffer
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

    using ScriptFieldMap = std::unordered_map<std::string, ScriptFieldInstance>;    // �ֶ��� - �ֶ�ʵ��

    class ScriptInstance;

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
        static Ref<ScriptInstance> GetMonoBehaviourScriptInstance(UUID objectID);
        static Ref<ScriptClass> GetMonoBehaviourClass(const std::string& name);
        static std::unordered_map<std::string, Ref<ScriptClass>> GetMonoBehaviourClasses();
        static ScriptFieldMap& GetScriptFieldMap(UUID objectID);

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
        friend class ScriptEngine;
    private:
        std::string m_ClassNamespace;   // �����ռ���
        std::string m_ClassName;        // ����

        std::map<std::string, ScriptField> m_Fields;  // �����ֶ� Map

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

        const std::map<std::string, ScriptField>& GetFields() const { return m_Fields; }
    };

    /// <summary>
    /// �ű�ʵ������¼ C# �ű�ʵ������Ϣ������ʵ������
    /// </summary>
    class ScriptInstance
    {
    private:
        friend class ScriptEngine;
        friend struct ScriptFieldInstance;
    private:
        Ref<ScriptClass> m_ScriptClass;     // �ű���

        MonoObject* m_Instance = nullptr;   // ��Ӧ�� Mono ʵ��

        MonoMethod* m_Constructor = nullptr;    // ���췽��
        MonoMethod* m_AwakeMethod = nullptr;    // Awake ����
        MonoMethod* m_UpdateMethod = nullptr;   // Update ����

        inline static char s_FieldValueBuffer[16];
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
        /// ��ȡ�ֶ��ڲ� (Mono) ֵ
        /// </summary>
        /// <param name="name">�ֶ���</param>
        /// <param name="buffer">�ֶ�ֵ������</param>
        /// <returns></returns>
        bool GetFieldValueInternal(const std::string& name, void* buffer);
        bool SetFieldValueInternal(const std::string& name, const void* value);
    };
}