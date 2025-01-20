#include "InspectorPanel.h"

// #include <imgui/imgui.h>
// #include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

#include "Lucky/Scene/Components/SelfComponent.h"
#include "Lucky/Scene/Components/TransformComponent.h"
#include "Lucky/Scene/Components/CameraComponent.h"
#include "Lucky/Scene/Components/SpriteRendererComponent.h"
#include "Lucky/Scene/Components/Rigidbody2DComponent.h"
#include "Lucky/Scene/Components/BoxCollider2DComponent.h"
#include "Lucky/Scene/Components/CircleCollider2DComponent.h"
#include "Lucky/Scene/Components/ScriptComponent.h"

#include "Lucky/Script/ScriptEngine.h"

#include "Lucky/ImGui/GUI.h"

#include "Lucky/Utils/PlatformUtils.h"

#include <filesystem>
#include <regex>

namespace Lucky
{
    namespace Utils
    {
        /// <summary>
        /// �� C# �ű��л�ȡ namespace ����
        /// </summary>
        /// <param name="filepath">�ļ�·��</param>
        /// <returns></returns>
        static std::string GetNamespaceFromScript(const std::filesystem::path& filepath)
        {
            std::ifstream stream(filepath, std::ios::in);

            // ��ʧ��
            if (!stream)
            {
                return "";
            }

            std::string line;
            std::regex namespaceRegex(R"(namespace\s+([a-zA-Z_][a-zA-Z0-9_\.]*))");
            std::smatch match;

            // ���ж�ȡ�ļ�����
            while (std::getline(stream, line))
            {
                // ʹ��������ʽƥ�� namespace ����
                if (std::regex_search(line, match, namespaceRegex))
                {
                    // ���������
                    if (match.size() > 1)
                    {
                        return match[1].str(); // ����ƥ�䵽�� namespace ����
                    }
                }
            }

            // û�ҵ� namespace
            return "";
        }
    }

    InspectorPanel::InspectorPanel()
    {
        m_SettingsButtonIcon = Texture2D::Create("Resources/Icons/SettingsButton_Icon.png");

        m_TransformIcon = Texture2D::Create("Resources/Icons/Components/Transform_Icon.png");
        m_CameraIcon = Texture2D::Create("Resources/Icons/Components/Camera_Icon.png");
        m_SpriteRendererIcon = Texture2D::Create("Resources/Icons/Components/SpriteRenderer_Icon.png");
        m_Rigidbody2DIcon = Texture2D::Create("Resources/Icons/Components/Rigidbody_Icon.png");
        m_BoxCollider2DIcon = Texture2D::Create("Resources/Icons/Components/BoxCollider2D_Icon.png");
        m_CircleCollider2DIcon = Texture2D::Create("Resources/Icons/Components/CircleCollider2D_Icon.png");
        m_ScriptIcon = Texture2D::Create("Resources/Icons/Components/CSharp_Icon.png");
    }

    InspectorPanel::InspectorPanel(const Ref<Scene>& scene)
        : m_Scene(scene)
    {
        m_SettingsButtonIcon = Texture2D::Create("Resources/Icons/SettingsButton_Icon.png");

        m_TransformIcon = Texture2D::Create("Resources/Icons/Components/Transform_Icon.png");
        m_CameraIcon = Texture2D::Create("Resources/Icons/Components/Camera_Icon.png");
        m_SpriteRendererIcon = Texture2D::Create("Resources/Icons/Components/SpriteRenderer_Icon.png");
        m_Rigidbody2DIcon = Texture2D::Create("Resources/Icons/Components/Rigidbody_Icon.png");
        m_BoxCollider2DIcon = Texture2D::Create("Resources/Icons/Components/BoxCollider2D_Icon.png");
        m_CircleCollider2DIcon = Texture2D::Create("Resources/Icons/Components/CircleCollider2D_Icon.png");
        m_ScriptIcon = Texture2D::Create("Resources/Icons/Components/CSharp_Icon.png");
    }

    void InspectorPanel::SetSceneContext(const Ref<Scene>& scene)
    {
        // �������ó�����Ϣ
        m_Scene = scene;
        Selection::Object = {};
    }

    void InspectorPanel::OnImGuiRender(bool& isOpen)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 10));    // ���� Padding���ؼ��߽絽���ڱ߽�ľ��룩
        ImGui::Begin("Inspector"/*, &isOpen*/);
        {
            ImGui::PopStyleVar();

            m_IsFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 8));   // ��ֱ���Ϊ 4

            // ��ѡ�е�������� TODO ����ʲ���Ϣ����
            if (m_SelectionObject)
            {
                DrawComponents(m_SelectionObject);  // ������� UI
            }

            ImGui::PopStyleVar();
        }
        ImGui::End();
    }

    void InspectorPanel::DrawAddComponents(Object object)
    {
        float panelWidth = ImGui::GetWindowContentRegionWidth() + ImGui::GetStyle().WindowPadding.x * 2;    // �����

        // ������е�λ��
        float posX = (panelWidth - 300) * 0.5f;

        // ���ð�ť��λ��
        ImGui::SetCursorPosX(posX);

        // ������
        if (ImGui::Button("Add Component", ImVec2(300, 0)))
        {
            ImGui::OpenPopup("AddComponent");   // �򿪵�����
        }

        // ��Ⱦ������
        if (ImGui::BeginPopup("AddComponent"))
        {
            DrawAddComponentItemButton<CameraComponent>("Camera");
            DrawAddComponentItemButton<SpriteRendererComponent>("Sprite Renderer");
            DrawAddComponentItemButton<Rigidbody2DComponent>("Rigidbody 2D");
            DrawAddComponentItemButton<BoxCollider2DComponent>("Box Collider 2D");
            DrawAddComponentItemButton<CircleCollider2DComponent>("Circle Collider 2D");
            DrawAddComponentItemButton<ScriptComponent>("Script");

            ImGui::EndPopup();
        }
    }

    void InspectorPanel::DrawComponents(Object object)
    {
        // Self ���
        if (object.HasComponent<SelfComponent>())
        {
            auto& name = object.GetComponent<SelfComponent>().Name;   // ������

            char buffer[256];                               // ��������� buffer
            memset(buffer, 0, sizeof(buffer));              // �� buffer ����
            strcpy_s(buffer, sizeof(buffer), name.c_str()); // buffer = name

            ImGui::Dummy(ImVec2(0, 0)); // ͸���ؼ�
            ImGui::SameLine();

            ImFont* boldFont = ImGui::GetIO().Fonts->Fonts[0];  // ����
            ImGui::PushFont(boldFont);

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 10));  // ����һ���ؼ��ļ��
            // �����������������ݸı�ʱ
            if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
            {
                name = std::string(buffer); // ���� name ��Ϊ buffer
            }
            ImGui::PopStyleVar();

            ImGui::PopFont();
        }

        // Transform ���
        DrawComponent<TransformComponent>("Transform", object, [](TransformComponent& transformComponent)
        {
            auto& transform = transformComponent.Transform;

            GUI::DragFloat3("Position", transform.GetPosition());   // λ��
            GUI::DragFloat3("Rotation", transform.GetRotation());   // ��ת
            GUI::DragFloat3("Scale", transform.GetScale());         // ����
        });

        // Camera ���
        DrawComponent<CameraComponent>("Camera", object, [](CameraComponent& cameraComponent)
        {
            auto& camera = cameraComponent.Camera;

            GUI::Toggle("Main Camera", &cameraComponent.Primary); // ��������ÿ�

            const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };    // ͶӰ���ͣ�͸�� ���� 
            int currentProjectionType = (int)camera.GetProjectionType();                // ��ǰͶӰ����

            // ������ ѡ��ͶӰ����
            if (GUI::DropdownList("Projection", projectionTypeStrings, 2, &currentProjectionType))
            {
                camera.SetProjectionType((SceneCamera::ProjectionType)currentProjectionType);   // �������ͶӰ����
            }

            // ͸��ͶӰ
            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
            {
                // FOV ��ֱ�Ž�
                float fov = camera.GetFOV();
                if (GUI::SliderAngle("Vertical FOV", &fov, 1.0f, 179.0f))
                {
                    camera.SetFOV(fov);
                }
            }

            // ����ͶӰ
            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
            {
                // �ߴ�
                float size = camera.GetSize();
                if (GUI::DragFloat("Size", &size))
                {
                    camera.SetSize(size);
                }
            }

            // ���ü�ƽ��
            float nearClip = camera.GetNearClip();
            if (GUI::DragFloat("Near", &nearClip, 0.01f, 0.01f, camera.GetFarClip() - 0.01f))
            {
                camera.SetNearClip(nearClip);
            }

            // Զ�ü�ƽ��
            float farClip = camera.GetFarClip();
            if (GUI::DragFloat("Far", &farClip, 0.01f, camera.GetNearClip() + 0.01f, 1000.0f))
            {
                camera.SetFarClip(farClip);
            }
        });

        // SpriteRenderer ���
        DrawComponent<SpriteRendererComponent>("Sprite Renderer", object, [](SpriteRendererComponent& spriteRendererComponent)
        {
            uint32_t spriteID = spriteRendererComponent.Sprite->GetRendererID();    // Sprite ID
            // ѡ��ͼƬ
            if (GUI::ImageButton("Sprite", spriteID, { 50, 50 }))
            {
                std::string filepath = FileDialogs::OpenFile("Sprite(*.png)\0*.png\0"); // TODO .sprite in project
                
                if (!filepath.empty())
                {
                    spriteRendererComponent.Sprite = Texture2D::Create(filepath);   // ���� Texture
                }
            }

            GUI::ColorEditor4("Color", spriteRendererComponent.Color);   // ��ɫ�༭��
        });

        // ���� Rigidbody2D ���
        DrawComponent<Rigidbody2DComponent>("Rigidbody 2D", object, [](Rigidbody2DComponent& rigidbody2DComponent)
        {
            Rigidbody2D& rigidbody2D = rigidbody2DComponent.Rigidbody2d;

            // ��������
            const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };   // �������ͣ���̬ ��̬ ����ѧ 
            int currentBodyType = (int)rigidbody2D.Type;                            // ��ǰ��������
           
            if (GUI::DropdownList("Body Type", bodyTypeStrings, 3, &currentBodyType))
            {
                rigidbody2D.Type = (Rigidbody2D::BodyType)currentBodyType;  // ���ø�������
            }

            GUI::DragFloat("Mass", &rigidbody2D.Mass, 0.01f, 0.01f, 1000000.0f);
            GUI::DragFloat("Linear Drag", &rigidbody2D.LinearDrag, 0.01f, 0.0f, 1000000.0f);
            GUI::DragFloat("Angular Drag", &rigidbody2D.AngularDrag, 0.01f, 0.0f, 1000000.0f);
            GUI::DragFloat("Gravity Scale", &rigidbody2D.GravityScale, 0.01f, 0.0f, 1000000.0f);

            // ��ײ���ģʽ
            const char* collisionDetectionStrings[] = { "Discrete", "Continuous" }; // ��ײ���ģʽ����ɢ ����
            int currentCollisionDetection = (int)rigidbody2D.CollisionDetection;    // ��ǰ��ײ���ģʽ

            if (GUI::DropdownList("Collision Detection", collisionDetectionStrings, 2, &currentCollisionDetection))
            {
                rigidbody2D.CollisionDetection = (Rigidbody2D::CollisionDetectionMode)currentCollisionDetection;    // ������ײ���ģʽ
            }

            GUI::Toggle("Freeze Rotation", &rigidbody2D.FreezeRotation);    // �Ƿ񶳽���ת Z �� ��ѡ��
        });

        // ���� BoxCollider2D ���
        DrawComponent<BoxCollider2DComponent>("BoxCollider 2D", object, [](BoxCollider2DComponent& boxCollider2DComponent)
        {
            BoxCollider2D& boxCollider2D = boxCollider2DComponent.BoxCollider2d;

            GUI::DragFloat2("Offset", boxCollider2D.Offset);
            GUI::DragFloat2("Size", boxCollider2D.Size, 0.01, 0.01f, 1000000.0f);

            GUI::DragFloat("Density", &boxCollider2D.Density, 0.01f, 0.0f, 1000000.0f);
            GUI::DragFloat("Friction", &boxCollider2D.Friction, 0.01f, 0.0f, 1.0f);
        });

        // ���� CircleCollider2D ���
        DrawComponent<CircleCollider2DComponent>("CircleCollider 2D", object, [](CircleCollider2DComponent& circleCollider2DComponent)
        {
            CircleCollider2D& circleCollider2D = circleCollider2DComponent.CircleCollider2d;

            GUI::DragFloat2("Offset", circleCollider2D.Offset);
            GUI::DragFloat("Radius", &circleCollider2D.Radius, 0.01f, 0.01f, 1000000.0f);

            GUI::DragFloat("Density", &circleCollider2D.Density, 0.01f, 0.0f, 1000000.0f);
            GUI::DragFloat("Friction", &circleCollider2D.Friction, 0.01f, 0.0f, 1.0f);
        });

        // ���� C# Script ���
        static std::string scriptComponentName;
        DrawComponent<ScriptComponent>(scriptComponentName + " (Script)", object, [&](ScriptComponent& scriptComponent)
        {
            scriptComponentName = scriptComponent.ClassName;    // �����ʾ�Ľű���
            std::string fullName = std::format("{}.{}", scriptComponent.ClassNamespace, scriptComponent.ClassName);

            uint32_t iconID = m_ScriptIcon->GetRendererID();

            GUI::ObjectField("Script", iconID, scriptComponent.ClassName, [&]()
            {
                // ѡ�� C# �ű��ļ�
                std::string filepath = FileDialogs::OpenFile("C# Script(*.cs)\0*.cs\0"); // TODO .Script in project

                if (!filepath.empty())
                {
                    // Temp TODO
                    // ��ȡ namespace ����
                    std::string classNamespace = Utils::GetNamespaceFromScript(filepath);
                    // ��ȡ class ��
                    size_t lastSlashPos = filepath.find_last_of("\\/") + 1;
                    size_t lastDotPos = filepath.find_last_of(".");
                    std::string className = filepath.substr(lastSlashPos, lastDotPos - lastSlashPos);

                    scriptComponent.ClassNamespace = classNamespace;    // �ű������ռ���
                    scriptComponent.ClassName = className;              // �ű�����
                }
            });

            if (m_Scene->IsRunning())
            {
                Ref<ScriptInstance> scriptInstance = ScriptEngine::GetMonoBehaviourScriptInstance(object.GetUUID());
                if (scriptInstance)
                {
                    // ���� public �ֶ�
                    const auto& fields = scriptInstance->GetScriptClass()->GetFields();
                    for (const auto& [name, field] : fields)
                    {
                        // Float
                        if (field.Type == ScriptFieldType::Float)
                        {
                            float value = scriptInstance->GetFieldValue<float>(name);
                            if (GUI::DragFloat(name, &value))
                            {
                                scriptInstance->SetFieldValue<float>(name, value);
                            }
                        }
                    }
                }
            }
            else
            {
                // Mono �ű��������
                if (ScriptEngine::MonoBehaviourClassExists(fullName))
                {
                    Ref<ScriptClass> monoBehaviourClass = ScriptEngine::GetMonoBehaviourClass(fullName);

                    const auto& fields = monoBehaviourClass->GetFields();   // C# �ű��е� public �ֶ�

                    ScriptFieldMap& fieldMap = ScriptEngine::GetScriptFieldMap(object.GetUUID());

                    for (const auto& [name, field] : fields)
                    {
                        // �� FieldMap �е� �ֶ�ֵ���õ� UI
                        if (fieldMap.find(name) != fieldMap.end())
                        {
                            ScriptFieldInstance& fieldInstance = fieldMap.at(name);

                            // Float
                            if (field.Type == ScriptFieldType::Float)
                            {
                                float value = fieldInstance.GetValue<float>();
                                if (GUI::DragFloat(name, &value))
                                {
                                    fieldInstance.SetValue(value);
                                }
                            }
                        }
                        // �� UI ��ȡֵ ���õ� FieldMap
                        else
                        {
                            ScriptFieldInstance& fieldInstance = fieldMap[name];

                            // TODO ��ʼ��ʾ�ű� public �ֶε�Ĭ��ֵ

                            // Float
                            if (field.Type == ScriptFieldType::Float)
                            {
                                float value = 0.0f;
                                if (GUI::DragFloat(name, &value))
                                {
                                    fieldInstance.Field = field;
                                    fieldInstance.SetValue(value);
                                }
                            }
                        }
                    }
                }
            }
        });

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 1));   // ��ֱ���Ϊ 1
        ImGui::Separator(); // �ָ���
        ImGui::PopStyleVar();
        ImGui::Separator(); // �ָ���

        DrawAddComponents(object); // ���������� UI
    }
}