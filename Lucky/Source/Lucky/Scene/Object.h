#pragma once

#include "Components/IDComponent.h"
#include "Components/SelfComponent.h"

#include "Scene.h"

#include "entt.hpp"

namespace Lucky
{
    /// <summary>
    /// ʵ��
    /// </summary>
    class Object
    {
    private:
        entt::entity m_ObjectID{ entt::null };  // ʵ�� ID
        Scene* m_Scene = nullptr;               // ʵ����������
    public:
        Object() {}
        Object(entt::entity objectID, Scene* scene);
        Object(const Object& other) = default;

        /// <summary>
        /// ��� T �������
        /// </summary>
        /// <typeparam name="T">�������</typeparam>
        /// <typeparam name="...Args">�����������</typeparam>
        /// <param name="...args">��������б�</param>
        /// <returns>���</returns>
        template<typename T, typename... Args>
        T& AddComponent(Args&&... args)
        {
            LC_CORE_ASSERT(!HasComponent<T>(), "Object already has component!");    // ������Ѵ���

            T& component = m_Scene->m_Registry.emplace<T>(m_ObjectID, std::forward<Args>(args)...); // �� m_Scene ������ʵ��ע������ T �������
            m_Scene->OnComponentAdded<T>(*this, component); // m_Scene �� this ������� T ���ʱ����

            return component;
        }

        /// <summary>
        /// ���� T �������
        /// </summary>
        /// <typeparam name="T">�������</typeparam>
        /// <returns>���</returns>
        template<typename T>
        T& GetComponent()
        {
            LC_CORE_ASSERT(HasComponent<T>(), "Object dose not have component!");   // �����������

            return m_Scene->m_Registry.get<T>(m_ObjectID);  // �� m_Scene ������ʵ��ע����� T �������
        }

        /// <summary>
        /// ��ѯ�Ƿ�ӵ�� T �������
        /// </summary>
        /// <typeparam name="T">�������</typeparam>
        /// <returns>��ѯ���</returns>
        template<typename T>
        bool HasComponent()
        {
            return m_Scene->m_Registry.has<T>(m_ObjectID);  // ���� m_Scene ������ m_ObjectID �� T �������
        }

        /// <summary>
        /// �Ƴ� T �������
        /// </summary>
        /// <typeparam name="T">�������</typeparam>
        template<typename T>
        void RemoveComponent()
        {
            LC_CORE_ASSERT(HasComponent<T>(), "Object dose not have component!");   // �����������

            m_Scene->m_Registry.remove<T>(m_ObjectID);  // �Ƴ� m_Scene ������ m_ObjectID �� T �������
        }

        operator bool() const { return m_ObjectID != entt::null; }
        operator entt::entity() const { return m_ObjectID; }
        operator uint32_t() const { return (uint32_t)m_ObjectID; }

        UUID GetUUID() { return GetComponent<IDComponent>().ID; }
        const std::string& GetName() { return GetComponent<SelfComponent>().Name; }

        bool operator==(const Object& other)
        {
            return m_ObjectID == other.m_ObjectID && m_Scene == other.m_Scene;  // ���� id ��ͬ && ����������ͬ
        }

        bool operator!=(const Object& other)
        {
            return !(*this == other);
        }
    };
}