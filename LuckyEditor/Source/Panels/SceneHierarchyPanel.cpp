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
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 6)); // ���� Padding���ؼ��߽絽���ڱ߽�ľ��룩
        ImGui::Begin(m_Name.c_str());
        {
            ImGui::PopStyleVar();

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 0));   // �������ڵ�֮��Ĵ�ֱ����Ϊ 0

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