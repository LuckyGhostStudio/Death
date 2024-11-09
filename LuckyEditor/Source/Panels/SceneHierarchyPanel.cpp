#include "SceneHierarchyPanel.h"

#include "Lucky/Core/Log.h"
#include "Lucky/Input/Input.h"
#include "Lucky/Scene/Components/SelfComponent.h"
#include "Lucky/Scene/Selection.h"

#include <imgui/imgui.h>

namespace Lucky
{
    SceneHierarchyPanel::SceneHierarchyPanel()
        : EditorWindow("Hierarchy")
    {

    }

    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
        : EditorWindow("Hierarchy")
    {
        SetScene(scene);
    }

    SceneHierarchyPanel::~SceneHierarchyPanel()
    {

    }

    void SceneHierarchyPanel::SetScene(const Ref<Scene>& scene)
    {
        m_Scene = scene;
    }

    void SceneHierarchyPanel::OnUpdate(DeltaTime dt)
    {

    }

    void SceneHierarchyPanel::OnImGuiRender()
    {
        ImGui::Begin(m_Name.c_str());
        {
            // ������������ʵ�壬������ each �ڵĺ���
            m_Scene->m_Registry.each([&](auto ObjectID)
            {
                Object object{ ObjectID, m_Scene.get() };

                DrawObjectNode(object); // ����������
            });

            // �����ͣ�ڸô��� && ������ ������հ�λ�ã�
            if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            {
                Selection::Object = {}; // ȡ��ѡ�У��ÿ�ѡ������
            }

            // �������� �Ҽ�������ڰ����򵯳��˵���- �Ҽ� ������������
            if (ImGui::BeginPopupContextWindow(0, 1, false))
            {
                // �˵������������
                if (ImGui::MenuItem("Create Empty Object"))
                {
                    m_Scene->CreateObject("Empty Object");  // ����������
                }

                ImGui::EndPopup();
            }
        }
        ImGui::End();
    }

    void SceneHierarchyPanel::DrawObjectNode(Object object)
    {
        auto& name = object.GetComponent<SelfComponent>().ObjectName;   // ������

        // ������־
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        
        if (Selection::Object == object)
        {
            flags |= ImGuiTreeNodeFlags_Selected;
        }

        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)object, flags, name.c_str());    // ���ڵ㣺��� id ����־ �������ʵ������

        // ����㱻���
        if (ImGui::IsItemClicked())
        {
            Selection::Object = object; // ����ѡ������

            LC_TRACE("��ǰѡ�����壺{0} {1}", Selection::Object.GetComponent<SelfComponent>().ObjectName, (uint32_t)Selection::Object);
        }

        // ɾ������
        bool objectDeleted = false;     // �����Ƿ���ɾ��
        // �Ҽ������������
        if (ImGui::BeginPopupContextItem())
        {
            // �˵��ɾ������
            if (ImGui::MenuItem("Delete Object"))
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

    void SceneHierarchyPanel::OnEvent(Event& e)
    {

    }
}