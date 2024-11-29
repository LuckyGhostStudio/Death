#include "SceneHierarchyPanel.h"

#include "Lucky/Core/Application.h"
#include "Lucky/Core/Log.h"
#include "Lucky/Core/Input/Input.h"
#include "Lucky/Scene/Components/SelfComponent.h"
#include "Lucky/Scene/Components/SpriteRendererComponent.h"
#include "Lucky/Scene/Components/CameraComponent.h"
#include "Lucky/Scene/Components/Rigidbody2DComponent.h"
#include "Lucky/Scene/Components/BoxCollider2DComponent.h"
#include "Lucky/Scene/Selection.h"

#include <imgui/imgui.h>

namespace Lucky
{
    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
        : m_Scene(scene)
    {

    }

    void SceneHierarchyPanel::SetSceneContext(const Ref<Scene>& scene)
    {
        // �������ó�����Ϣ
        m_Scene = scene;
        Selection::Object = {};
    }

    void SceneHierarchyPanel::OnImGuiRender(bool& isOpen)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 6)); // ���� Padding���ؼ��߽絽���ڱ߽�ľ��룩
        ImGui::Begin("Hierarchy"/*, &isOpen*/);
        {
            ImGui::PopStyleVar();

            m_IsFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 0));   // �������ڵ�֮��Ĵ�ֱ����Ϊ 0

            if (m_Scene)
            {
                // TODO ��ӳ������ڵ� ��������״̬��ʾ Name *
                // ������������ʵ�壬������ each �ڵĺ���
                m_Scene->m_Registry.each([&](auto ObjectID)
                {
                    Object object{ ObjectID, m_Scene.get() };

                    DrawObjectNode(object); // ����������
                });


                ImGui::PopStyleVar();

                // ������ && �����ͣ�ڸô��ڣ�����հ�λ�ã�
                if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered())
                {
                    Selection::Object = {}; // ȡ��ѡ�У��ÿ�ѡ������
                }

                // �������� �Ҽ�������ڰ����򵯳��˵���- �Ҽ� ������������
                if (ImGui::BeginPopupContextWindow(nullptr, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
                {
                    Object newObject;

                    // ����������
                    if (ImGui::MenuItem("Create Empty"))
                    {
                        m_Scene->CreateObject("Object");
                    }
                    // ���� Sprite
                    if (ImGui::MenuItem("Sprite"))
                    {
                        newObject = m_Scene->CreateObject("Sprite");
                        newObject.AddComponent<SpriteRendererComponent>();
                    }
                    // ���� Camera
                    if (ImGui::MenuItem("Camera"))
                    {
                        newObject = m_Scene->CreateObject("Camera");
                        newObject.AddComponent<CameraComponent>();
                    }
                    // ���� 2D ���� TODO Test
                    if (ImGui::MenuItem("Rigidbody 2D"))
                    {
                        newObject = m_Scene->CreateObject("Rigidbody 2D");
                        newObject.AddComponent<SpriteRendererComponent>();
                        newObject.AddComponent<Rigidbody2DComponent>();
                        newObject.AddComponent<BoxCollider2DComponent>();
                    }

                    ImGui::EndPopup();
                }
            }
        }
        ImGui::End();
    }

    void SceneHierarchyPanel::DrawObjectNode(Object object)
    {
        std::string& name = object.GetComponent<SelfComponent>().Name;  // ������

        // ������־
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        
        static ImVec4 headerColor = { 0.38f, 0.3805f, 0.381f, 1.0f }; // ���ڵ���ɫ

        if (Selection::Object == object)
        {
            flags |= ImGuiTreeNodeFlags_Selected;

            headerColor = { 0.2588f, 0.5882f, 0.9804f, 0.3490f };   // ����ѡ����ɫ
        }
        else
        {
            headerColor = { 0.38f, 0.3805f, 0.381f, 1.0f };
        }

        ImGui::PushStyleColor(ImGuiCol_Header, headerColor);
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, headerColor);
        ImGui::PushStyleColor(ImGuiCol_HeaderActive, headerColor);

        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)object, flags, name.c_str());    // ���ڵ㣺��� id ����־ �������ʵ������
        
        ImGui::PopStyleColor(3);

        // ����㱻���
        if (ImGui::IsItemClicked())
        {
            Selection::Object = object; // ����ѡ������

            LC_TRACE("Selected Object��[ENTT = {0}, UUID {1}, Name {2}]", (uint32_t)Selection::Object, Selection::Object.GetUUID(), Selection::Object.GetName());
        }

        // ɾ������
        bool objectDeleted = false;     // �����Ƿ���ɾ��
        // �Ҽ������������
        if (ImGui::BeginPopupContextItem())
        {
            // �˵��ɾ������
            if (ImGui::MenuItem("Delete"))
            {
                objectDeleted = true;   // ������Ϊ��ɾ������Ⱦ��������� UI ��ɾ��������
            }

            ImGui::EndPopup();
        }

        // ������Ѵ�
        if (opened)
        {
            // TODO �ӽڵ�

            ImGui::TreePop();
        }

        if (objectDeleted)
        {
            m_Scene->DeleteObject(object);  // ɾ������

            // ɾ��������Ϊ��ѡ������
            if (Selection::Object == object)
            {
                Selection::Object = {};     // �����ѡ������
            }
        }
    }

    void SceneHierarchyPanel::OnDuplicateObject()
    {
        if (Selection::Object)
        {
            m_Scene->DuplicateObject(Selection::Object);    // ���Ƶ�ǰѡ������
        }
    }

    void SceneHierarchyPanel::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<KeyPressedEvent>(LC_BIND_EVENT_FUNC(SceneHierarchyPanel::OnKeyPressed));    // ���ð��������¼�
    }

    bool SceneHierarchyPanel::OnKeyPressed(KeyPressedEvent& e)
    {
        // ��ݼ����ظ����� == 0
        if (e.GetRepeatCount() > 0)
        {
            return false;
        }

        bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl); // Ctrl ������
        bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);       // Shift ������

        switch (e.GetKeyCode())
        {
            case Key::D:
                if (control)
                {
                    OnDuplicateObject();    // Ctrl+D ��������
                }
                break;
        }

        return false;
    }
}