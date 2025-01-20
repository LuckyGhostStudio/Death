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
        /// 从 C# 脚本中获取 namespace 名称
        /// </summary>
        /// <param name="filepath">文件路径</param>
        /// <returns></returns>
        static std::string GetNamespaceFromScript(const std::filesystem::path& filepath)
        {
            std::ifstream stream(filepath, std::ios::in);

            // 打开失败
            if (!stream)
            {
                return "";
            }

            std::string line;
            std::regex namespaceRegex(R"(namespace\s+([a-zA-Z_][a-zA-Z0-9_\.]*))");
            std::smatch match;

            // 按行读取文件内容
            while (std::getline(stream, line))
            {
                // 使用正则表达式匹配 namespace 声明
                if (std::regex_search(line, match, namespaceRegex))
                {
                    // 捕获组存在
                    if (match.size() > 1)
                    {
                        return match[1].str(); // 返回匹配到的 namespace 名称
                    }
                }
            }

            // 没找到 namespace
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
        // 重新设置场景信息
        m_Scene = scene;
        Selection::Object = {};
    }

    void InspectorPanel::OnImGuiRender(bool& isOpen)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 10));    // 窗口 Padding（控件边界到窗口边界的距离）
        ImGui::Begin("Inspector"/*, &isOpen*/);
        {
            ImGui::PopStyleVar();

            m_IsFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 8));   // 垂直间距为 4

            // 被选中的物体存在 TODO 添加资产信息绘制
            if (m_SelectionObject)
            {
                DrawComponents(m_SelectionObject);  // 绘制组件 UI
            }

            ImGui::PopStyleVar();
        }
        ImGui::End();
    }

    void InspectorPanel::DrawAddComponents(Object object)
    {
        float panelWidth = ImGui::GetWindowContentRegionWidth() + ImGui::GetStyle().WindowPadding.x * 2;    // 面板宽度

        // 计算居中的位置
        float posX = (panelWidth - 300) * 0.5f;

        // 设置按钮的位置
        ImGui::SetCursorPosX(posX);

        // 添加组件
        if (ImGui::Button("Add Component", ImVec2(300, 0)))
        {
            ImGui::OpenPopup("AddComponent");   // 打开弹出框
        }

        // 渲染弹出框
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
        // Self 组件
        if (object.HasComponent<SelfComponent>())
        {
            auto& name = object.GetComponent<SelfComponent>().Name;   // 物体名

            char buffer[256];                               // 输入框内容 buffer
            memset(buffer, 0, sizeof(buffer));              // 将 buffer 置零
            strcpy_s(buffer, sizeof(buffer), name.c_str()); // buffer = name

            ImGui::Dummy(ImVec2(0, 0)); // 透明控件
            ImGui::SameLine();

            ImFont* boldFont = ImGui::GetIO().Fonts->Fonts[0];  // 粗体
            ImGui::PushFont(boldFont);

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 10));  // 和下一个控件的间距
            // 创建输入框，输入框内容改变时
            if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
            {
                name = std::string(buffer); // 物体 name 设为 buffer
            }
            ImGui::PopStyleVar();

            ImGui::PopFont();
        }

        // Transform 组件
        DrawComponent<TransformComponent>("Transform", object, [](TransformComponent& transformComponent)
        {
            auto& transform = transformComponent.Transform;

            GUI::DragFloat3("Position", transform.GetPosition());   // 位置
            GUI::DragFloat3("Rotation", transform.GetRotation());   // 旋转
            GUI::DragFloat3("Scale", transform.GetScale());         // 缩放
        });

        // Camera 组件
        DrawComponent<CameraComponent>("Camera", object, [](CameraComponent& cameraComponent)
        {
            auto& camera = cameraComponent.Camera;

            GUI::Toggle("Main Camera", &cameraComponent.Primary); // 主相机设置框

            const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };    // 投影类型：透视 正交 
            int currentProjectionType = (int)camera.GetProjectionType();                // 当前投影类型

            // 下拉框 选择投影类型
            if (GUI::DropdownList("Projection", projectionTypeStrings, 2, &currentProjectionType))
            {
                camera.SetProjectionType((SceneCamera::ProjectionType)currentProjectionType);   // 设置相机投影类型
            }

            // 透视投影
            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
            {
                // FOV 垂直张角
                float fov = camera.GetFOV();
                if (GUI::SliderAngle("Vertical FOV", &fov, 1.0f, 179.0f))
                {
                    camera.SetFOV(fov);
                }
            }

            // 正交投影
            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
            {
                // 尺寸
                float size = camera.GetSize();
                if (GUI::DragFloat("Size", &size))
                {
                    camera.SetSize(size);
                }
            }

            // 近裁剪平面
            float nearClip = camera.GetNearClip();
            if (GUI::DragFloat("Near", &nearClip, 0.01f, 0.01f, camera.GetFarClip() - 0.01f))
            {
                camera.SetNearClip(nearClip);
            }

            // 远裁剪平面
            float farClip = camera.GetFarClip();
            if (GUI::DragFloat("Far", &farClip, 0.01f, camera.GetNearClip() + 0.01f, 1000.0f))
            {
                camera.SetFarClip(farClip);
            }
        });

        // SpriteRenderer 组件
        DrawComponent<SpriteRendererComponent>("Sprite Renderer", object, [](SpriteRendererComponent& spriteRendererComponent)
        {
            uint32_t spriteID = spriteRendererComponent.Sprite->GetRendererID();    // Sprite ID
            // 选择图片
            if (GUI::ImageButton("Sprite", spriteID, { 50, 50 }))
            {
                std::string filepath = FileDialogs::OpenFile("Sprite(*.png)\0*.png\0"); // TODO .sprite in project
                
                if (!filepath.empty())
                {
                    spriteRendererComponent.Sprite = Texture2D::Create(filepath);   // 创建 Texture
                }
            }

            GUI::ColorEditor4("Color", spriteRendererComponent.Color);   // 颜色编辑器
        });

        // 绘制 Rigidbody2D 组件
        DrawComponent<Rigidbody2DComponent>("Rigidbody 2D", object, [](Rigidbody2DComponent& rigidbody2DComponent)
        {
            Rigidbody2D& rigidbody2D = rigidbody2DComponent.Rigidbody2d;

            // 刚体类型
            const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };   // 刚体类型：动态 静态 动力学 
            int currentBodyType = (int)rigidbody2D.Type;                            // 当前刚体类型
           
            if (GUI::DropdownList("Body Type", bodyTypeStrings, 3, &currentBodyType))
            {
                rigidbody2D.Type = (Rigidbody2D::BodyType)currentBodyType;  // 设置刚体类型
            }

            GUI::DragFloat("Mass", &rigidbody2D.Mass, 0.01f, 0.01f, 1000000.0f);
            GUI::DragFloat("Linear Drag", &rigidbody2D.LinearDrag, 0.01f, 0.0f, 1000000.0f);
            GUI::DragFloat("Angular Drag", &rigidbody2D.AngularDrag, 0.01f, 0.0f, 1000000.0f);
            GUI::DragFloat("Gravity Scale", &rigidbody2D.GravityScale, 0.01f, 0.0f, 1000000.0f);

            // 碰撞检测模式
            const char* collisionDetectionStrings[] = { "Discrete", "Continuous" }; // 碰撞检测模式：离散 连续
            int currentCollisionDetection = (int)rigidbody2D.CollisionDetection;    // 当前碰撞检测模式

            if (GUI::DropdownList("Collision Detection", collisionDetectionStrings, 2, &currentCollisionDetection))
            {
                rigidbody2D.CollisionDetection = (Rigidbody2D::CollisionDetectionMode)currentCollisionDetection;    // 设置碰撞检测模式
            }

            GUI::Toggle("Freeze Rotation", &rigidbody2D.FreezeRotation);    // 是否冻结旋转 Z 轴 勾选框
        });

        // 绘制 BoxCollider2D 组件
        DrawComponent<BoxCollider2DComponent>("BoxCollider 2D", object, [](BoxCollider2DComponent& boxCollider2DComponent)
        {
            BoxCollider2D& boxCollider2D = boxCollider2DComponent.BoxCollider2d;

            GUI::DragFloat2("Offset", boxCollider2D.Offset);
            GUI::DragFloat2("Size", boxCollider2D.Size, 0.01, 0.01f, 1000000.0f);

            GUI::DragFloat("Density", &boxCollider2D.Density, 0.01f, 0.0f, 1000000.0f);
            GUI::DragFloat("Friction", &boxCollider2D.Friction, 0.01f, 0.0f, 1.0f);
        });

        // 绘制 CircleCollider2D 组件
        DrawComponent<CircleCollider2DComponent>("CircleCollider 2D", object, [](CircleCollider2DComponent& circleCollider2DComponent)
        {
            CircleCollider2D& circleCollider2D = circleCollider2DComponent.CircleCollider2d;

            GUI::DragFloat2("Offset", circleCollider2D.Offset);
            GUI::DragFloat("Radius", &circleCollider2D.Radius, 0.01f, 0.01f, 1000000.0f);

            GUI::DragFloat("Density", &circleCollider2D.Density, 0.01f, 0.0f, 1000000.0f);
            GUI::DragFloat("Friction", &circleCollider2D.Friction, 0.01f, 0.0f, 1.0f);
        });

        // 绘制 C# Script 组件
        static std::string scriptComponentName;
        DrawComponent<ScriptComponent>(scriptComponentName + " (Script)", object, [&](ScriptComponent& scriptComponent)
        {
            scriptComponentName = scriptComponent.ClassName;    // 组件显示的脚本名
            std::string fullName = std::format("{}.{}", scriptComponent.ClassNamespace, scriptComponent.ClassName);

            uint32_t iconID = m_ScriptIcon->GetRendererID();

            GUI::ObjectField("Script", iconID, scriptComponent.ClassName, [&]()
            {
                // 选择 C# 脚本文件
                std::string filepath = FileDialogs::OpenFile("C# Script(*.cs)\0*.cs\0"); // TODO .Script in project

                if (!filepath.empty())
                {
                    // Temp TODO
                    // 获取 namespace 名称
                    std::string classNamespace = Utils::GetNamespaceFromScript(filepath);
                    // 获取 class 名
                    size_t lastSlashPos = filepath.find_last_of("\\/") + 1;
                    size_t lastDotPos = filepath.find_last_of(".");
                    std::string className = filepath.substr(lastSlashPos, lastDotPos - lastSlashPos);

                    scriptComponent.ClassNamespace = classNamespace;    // 脚本命名空间名
                    scriptComponent.ClassName = className;              // 脚本类名
                }
            });

            if (m_Scene->IsRunning())
            {
                Ref<ScriptInstance> scriptInstance = ScriptEngine::GetMonoBehaviourScriptInstance(object.GetUUID());
                if (scriptInstance)
                {
                    // 所有 public 字段
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
                // Mono 脚本子类存在
                if (ScriptEngine::MonoBehaviourClassExists(fullName))
                {
                    Ref<ScriptClass> monoBehaviourClass = ScriptEngine::GetMonoBehaviourClass(fullName);

                    const auto& fields = monoBehaviourClass->GetFields();   // C# 脚本中的 public 字段

                    ScriptFieldMap& fieldMap = ScriptEngine::GetScriptFieldMap(object.GetUUID());

                    for (const auto& [name, field] : fields)
                    {
                        // 将 FieldMap 中的 字段值设置到 UI
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
                        // 从 UI 读取值 设置到 FieldMap
                        else
                        {
                            ScriptFieldInstance& fieldInstance = fieldMap[name];

                            // TODO 初始显示脚本 public 字段的默认值

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

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 1));   // 垂直间距为 1
        ImGui::Separator(); // 分隔线
        ImGui::PopStyleVar();
        ImGui::Separator(); // 分隔线

        DrawAddComponents(object); // 绘制添加组件 UI
    }
}