#include "lcpch.h"
#include "SceneSerializer.h"

#include "Object.h"

#include "Lucky/Scene/Components/SelfComponent.h"
#include "Lucky/Scene/Components/TransformComponent.h"
#include "Lucky/Scene/Components/SpriteRendererComponent.h"
#include "Lucky/Scene/Components/CameraComponent.h"
#include "Lucky/Scene/Components/Rigidbody2DComponent.h"
#include "Lucky/Scene/Components/BoxCollider2DComponent.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace YAML
{
    /// <summary>
    /// vec2 ת��
    /// </summary>
    template<>
    struct convert<glm::vec2>
    {
        /// <summary>
        /// �� vec2 ת��Ϊ YAML �Ľڵ�
        /// </summary>
        /// <param name="rhs">vec2 ����</param>
        /// <returns>���</returns>
        static Node encode(const glm::vec2& rhs)
        {
            Node node;

            node.push_back(rhs.x);
            node.push_back(rhs.y);

            node.SetStyle(EmitterStyle::Flow);

            return node;
        }

        /// <summary>
        /// �� YAML �������ת��Ϊ vec2
        /// </summary>
        /// <param name="node">���</param>
        /// <param name="rhs">vec2</param>
        /// <returns>�Ƿ�ת���ɹ�</returns>
        static bool decode(const Node& node, glm::vec2& rhs)
        {
            if (!node.IsSequence() || node.size() != 2)
            {
                return false;
            }

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();

            return true;
        }
    };

    /// <summary>
    /// vec3 ת��
    /// </summary>
    template<>
    struct convert<glm::vec3>
    {
        /// <summary>
        /// �� vec3 ת��Ϊ YAML �Ľڵ�
        /// </summary>
        /// <param name="rhs">vec3 ����</param>
        /// <returns>���</returns>
        static Node encode(const glm::vec3& rhs)
        {
            Node node;

            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);

            node.SetStyle(EmitterStyle::Flow);

            return node;
        }

        /// <summary>
        /// �� YAML �������ת��Ϊ vec3
        /// </summary>
        /// <param name="node">���</param>
        /// <param name="rhs">vec3</param>
        /// <returns>�Ƿ�ת���ɹ�</returns>
        static bool decode(const Node& node, glm::vec3& rhs)
        {
            if (!node.IsSequence() || node.size() != 3)
            {
                return false;
            }

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();

            return true;
        }
    };

    /// <summary>
    /// vec4 ת��
    /// </summary>
    template<>
    struct convert<glm::vec4>
    {
        static Node encode(const glm::vec4& rhs)
        {
            Node node;

            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);

            node.SetStyle(EmitterStyle::Flow);

            return node;
        }

        static bool decode(const Node& node, glm::vec4& rhs)
        {
            if (!node.IsSequence() || node.size() != 4)
            {
                return false;
            }

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();

            return true;
        }
    };
}

namespace Lucky
{
    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
    {
        out << YAML::Flow;    // �� [x,y]
        out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;

        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;

        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;

        return out;
    }

    SceneSerializer::SceneSerializer(const Ref<Scene> scene)
        : m_Scene(scene)
    {

    }

    /// <summary>
    /// ���л�����
    /// </summary>
    /// <param name="out">������</param>
    /// <param name="entity">����</param>
    static void SerializeEntity(YAML::Emitter& out, Object object)
    {
        out << YAML::BeginMap;  // ��ʼ���� Map
        out << YAML::Key << "Object" << YAML::Value << "12837192831273"; // TODO: Object ID goes here

        // Self ���
        if (object.HasComponent<SelfComponent>())
        {
            out << YAML::Key << "SelfComponent";
            out << YAML::BeginMap;  // ��ʼ Self ��� Map

            SelfComponent& selfComponent = object.GetComponent<SelfComponent>(); // Self ���

            out << YAML::Key << "ObjectName" << YAML::Value << selfComponent.ObjectName;
            out << YAML::Key << "ObjectEnable" << YAML::Value << selfComponent.ObjectEnable;

            out << YAML::EndMap;    // ���� Self ��� Map
        }

        // Transform ���
        if (object.HasComponent<TransformComponent>())
        {
            out << YAML::Key << "TransformComponent";
            out << YAML::BeginMap;  // ��ʼ Transform ��� Map

            // Transform �������
            TransformComponent& transformComponent = object.GetComponent<TransformComponent>();
            Transform& transform = transformComponent.Transform;

            out << YAML::Key << "Position" << YAML::Value << transform.GetPosition();
            out << YAML::Key << "Rotation" << YAML::Value << transform.GetRotation();
            out << YAML::Key << "Scale" << YAML::Value << transform.GetScale();

            out << YAML::EndMap;    // ���� Transform ��� Map
        }

        // Camera ���
        if (object.HasComponent<CameraComponent>())
        {
            out << YAML::Key << "CameraComponent";
            out << YAML::BeginMap;  // ��ʼ Camera ��� Map

            CameraComponent& cameraComponent = object.GetComponent<CameraComponent>();
            SceneCamera& camera = cameraComponent.Camera;

            out << YAML::Key << "Primary" << YAML::Value << camera.IsPrimary();

            out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
            out << YAML::Key << "Size" << YAML::Value << camera.GetSize();
            out << YAML::Key << "FOV" << YAML::Value << camera.GetFOV();
            out << YAML::Key << "Near" << YAML::Value << camera.GetNearClip();
            out << YAML::Key << "Far" << YAML::Value << camera.GetFarClip();

            out << YAML::EndMap;    // ���� Camera ��� Map
        }

        // SpriteRenderer ���
        if (object.HasComponent<SpriteRendererComponent>())
        {
            out << YAML::Key << "SpriteRendererComponent";
            out << YAML::BeginMap;  // ��ʼ SpriteRenderer ��� Map

            SpriteRendererComponent& spriteRendererComponent = object.GetComponent<SpriteRendererComponent>();

            out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;
            // TODO Texture

            out << YAML::EndMap;    // ���� SpriteRenderer ��� Map
        }

        // Rigidbody2D ���
        if (object.HasComponent<Rigidbody2DComponent>())
        {
            out << YAML::Key << "Rigidbody2DComponent";
            out << YAML::BeginMap;  // Rigidbody2DComponent

            Rigidbody2DComponent& rigidbody2DComponent = object.GetComponent<Rigidbody2DComponent>();
            Rigidbody2D& rigidbody2D = rigidbody2DComponent.Rigidbody2d;

            out << YAML::Key << "BodyType" << YAML::Value << (int)rigidbody2D.GetBodyType();
            out << YAML::Key << "FreezeRotation" << YAML::Value << rigidbody2D.IsFreezeRotation();

            out << YAML::EndMap;    // Rigidbody2DComponent
        }

        // BoxCollider2D ���
        if (object.HasComponent<BoxCollider2DComponent>())
        {
            out << YAML::Key << "BoxCollider2DComponent";
            out << YAML::BeginMap;  // BoxCollider2DComponent

            BoxCollider2DComponent& boxCollider2DComponent = object.GetComponent<BoxCollider2DComponent>();
            BoxCollider2D& boxCollider2D = boxCollider2DComponent.BoxCollider2d;

            out << YAML::Key << "Offset" << YAML::Value << boxCollider2D.GetOffset();
            out << YAML::Key << "Size" << YAML::Value << boxCollider2D.GetSize();
            out << YAML::Key << "Density" << YAML::Value << boxCollider2D.GetDensity();
            out << YAML::Key << "Friction" << YAML::Value << boxCollider2D.GetFriction();
            out << YAML::Key << "Restitution" << YAML::Value << boxCollider2D.GetRestitution();
            out << YAML::Key << "RestitutionThreshold" << YAML::Value << boxCollider2D.GetRestitutionThreshold();

            out << YAML::EndMap;    // BoxCollider2DComponent
        }

        out << YAML::EndMap;    // �������� Map
    }

    void SceneSerializer::Serialize(const std::string& filepath)
    {
        YAML::Emitter out;      // ������

        out << YAML::BeginMap;  // ��ʼ���� Map
        out << YAML::Key << "Scene" << YAML::Value << "Untitled";       // ������������
        out << YAML::Key << "Objects" << YAML::Value << YAML::BeginSeq; // �������У���ʼ��������

        // ��������ע�����������
        m_Scene->m_Registry.each([&](auto objectID)
        {
            Object object = { objectID, m_Scene.get() };
            if (!object)
            {
                return;
            }

            SerializeEntity(out, object);   // ���л�����
        });

        out << YAML::EndSeq;    // ������������
        out << YAML::EndMap;    // �������� Map

        std::ofstream fout(filepath);   // �����
        fout << out.c_str();            // ������л������������ļ�
    }

    void SceneSerializer::SerializeRuntime(const std::string& filepath)
    {
        LC_CORE_ASSERT(false, "Not implemented!");  // δʵ��
    }

    bool SceneSerializer::Deserialize(const std::string& filepath)
    {
        YAML::Node data = YAML::LoadFile(filepath); // ���ص� YMAL ���

        // Scene �ڵ㲻����
        if (!data["Scene"])
        {
            return false;
        }

        std::string sceneName = data["Scene"].as<std::string>();    // ������

        LC_CORE_TRACE("Deserializing scene '{0}'", sceneName);

        YAML::Node objects = data["Objects"];   // �������н��

        // �������н�����
        if (objects)
        {
            // �����������������
            for (auto object : objects)
            {
                uint64_t uuid = object["Object"].as<uint64_t>();    // ���� uuid TODO
                
                std::string objectName;

                // Self ������
                YAML::Node selfComponentNode = object["SelfComponent"];
                if (selfComponentNode)
                {
                    objectName = selfComponentNode["ObjectName"].as<std::string>(); // ������
                    // TODO ObjectEnable
                }

                LC_CORE_TRACE("Deserialized object with ID = {0}, name = {1}", uuid, objectName);

                Object deserializedObject = m_Scene->CreateObject(objectName);  // �������� TODO ʹ�� uuid

                // Transform ������
                YAML::Node transformComponentNode = object["TransformComponent"];
                if (transformComponentNode)
                {
                    TransformComponent& transformComponent = deserializedObject.GetComponent<TransformComponent>();  // ��ȡ Transform ���

                    Transform& transform = transformComponent.Transform;

                    transform.SetPosition(transformComponentNode["Position"].as<glm::vec3>());
                    transform.SetRotation(transformComponentNode["Rotation"].as<glm::vec3>());
                    transform.SetScale(transformComponentNode["Scale"].as<glm::vec3>());
                }

                // Camera ������
                YAML::Node cameraComponentNode = object["CameraComponent"];
                if (cameraComponentNode)
                {
                    CameraComponent& cameraComponent = deserializedObject.AddComponent<CameraComponent>();  // ��� Camera ���

                    SceneCamera& camera = cameraComponent.Camera;

                    camera.SetProjectionType((SceneCamera::ProjectionType)cameraComponentNode["ProjectionType"].as<int>());

                    camera.SetSize(cameraComponentNode["Size"].as<float>());
                    camera.SetFOV(cameraComponentNode["FOV"].as<float>());
                    camera.SetNearClip(cameraComponentNode["Near"].as<float>());
                    camera.SetFarClip(cameraComponentNode["Far"].as<float>());
                    camera.SetPrimary(cameraComponentNode["Primary"].as<bool>());
                }

                // SpriteRenderer ������
                YAML::Node spriteRendererComponentNode = object["SpriteRendererComponent"];
                if (spriteRendererComponentNode)
                {
                    SpriteRendererComponent& spriteRendererComponent = deserializedObject.AddComponent<SpriteRendererComponent>();  // ��� SpriteRenderer ���

                    spriteRendererComponent.Color = spriteRendererComponentNode["Color"].as<glm::vec4>();
                }

                // Rigidbody2D ������
                YAML::Node rigidbody2DNode = object["Rigidbody2DComponent"];
                if (rigidbody2DNode)
                {
                    Rigidbody2DComponent& rigidbody2DComponent = deserializedObject.AddComponent<Rigidbody2DComponent>();       // ��� Rigidbody2D ���
                    Rigidbody2D& rigidbody2D = rigidbody2DComponent.Rigidbody2d;

                    rigidbody2D.SetBodyType((Rigidbody2D::BodyType)rigidbody2DNode["BodyType"].as<int>());
                    rigidbody2D.FreezeRotation(rigidbody2DNode["FreezeRotation"].as<bool>());
                }

                // BoxCollider2D ������
                YAML::Node boxCollider2DNode = object["BoxCollider2DComponent"];
                if (boxCollider2DNode)
                {
                    BoxCollider2DComponent& boxCollider2DComponent = deserializedObject.AddComponent<BoxCollider2DComponent>(); // ��� BoxCollider2D ���
                    BoxCollider2D& boxCollider2D = boxCollider2DComponent.BoxCollider2d;

                    boxCollider2D.SetOffset(boxCollider2DNode["Offset"].as<glm::vec2>());
                    boxCollider2D.SetSize(boxCollider2DNode["Size"].as<glm::vec2>());
                    boxCollider2D.SetDensity(boxCollider2DNode["Density"].as<float>());
                    boxCollider2D.SetFriction(boxCollider2DNode["Friction"].as<float>());
                    boxCollider2D.SetRestitution(boxCollider2DNode["Restitution"].as<float>());
                    boxCollider2D.SetRestitutionThreshold(boxCollider2DNode["RestitutionThreshold"].as<float>());
                }
            }
        }

        return true;
    }

    bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
    {
        LC_CORE_ASSERT(false, "Not implemented!");  // δʵ��

        return false;
    }
}