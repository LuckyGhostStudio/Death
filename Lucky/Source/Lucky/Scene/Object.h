#pragma once

#include "Components/IDComponent.h"
#include "Components/SelfComponent.h"

#include "Scene.h"

#include "entt.hpp"

namespace Lucky
{
    /// <summary>
    /// 实体
    /// </summary>
    class Object
    {
    private:
        entt::entity m_ObjectID{ entt::null };  // 实体 ID
        Scene* m_Scene = nullptr;               // 实体所属场景
    public:
        Object() {}
        Object(entt::entity objectID, Scene* scene);
        Object(const Object& other) = default;

        /// <summary>
        /// 添加 T 类型组件
        /// </summary>
        /// <typeparam name="T">组件类型</typeparam>
        /// <typeparam name="...Args">组件参数类型</typeparam>
        /// <param name="...args">组件参数列表</param>
        /// <returns>组件</returns>
        template<typename T, typename... Args>
        T& AddComponent(Args&&... args)
        {
            LC_CORE_ASSERT(!HasComponent<T>(), "Object already has component!");    // 该组件已存在

            T& component = m_Scene->m_Registry.emplace<T>(m_ObjectID, std::forward<Args>(args)...); // 向 m_Scene 场景的实体注册表添加 T 类型组件
            m_Scene->OnComponentAdded<T>(*this, component); // m_Scene 向 this 物体添加 T 组件时调用

            return component;
        }

        /// <summary>
        /// 返回 T 类型组件
        /// </summary>
        /// <typeparam name="T">组件类型</typeparam>
        /// <returns>组件</returns>
        template<typename T>
        T& GetComponent()
        {
            LC_CORE_ASSERT(HasComponent<T>(), "Object dose not have component!");   // 该组件不存在

            return m_Scene->m_Registry.get<T>(m_ObjectID);  // 从 m_Scene 场景的实体注册表获得 T 类型组件
        }

        /// <summary>
        /// 查询是否拥有 T 类型组件
        /// </summary>
        /// <typeparam name="T">组件类型</typeparam>
        /// <returns>查询结果</returns>
        template<typename T>
        bool HasComponent()
        {
            return m_Scene->m_Registry.has<T>(m_ObjectID);  // 查找 m_Scene 场景中 m_ObjectID 的 T 类型组件
        }

        /// <summary>
        /// 移除 T 类型组件
        /// </summary>
        /// <typeparam name="T">组件类型</typeparam>
        template<typename T>
        void RemoveComponent()
        {
            LC_CORE_ASSERT(HasComponent<T>(), "Object dose not have component!");   // 该组件不存在

            m_Scene->m_Registry.remove<T>(m_ObjectID);  // 移除 m_Scene 场景中 m_ObjectID 的 T 类型组件
        }

        operator bool() const { return m_ObjectID != entt::null; }
        operator entt::entity() const { return m_ObjectID; }
        operator uint32_t() const { return (uint32_t)m_ObjectID; }

        UUID GetUUID() { return GetComponent<IDComponent>().ID; }
        const std::string& GetName() { return GetComponent<SelfComponent>().Name; }

        bool operator==(const Object& other)
        {
            return m_ObjectID == other.m_ObjectID && m_Scene == other.m_Scene;  // 物体 id 相同 && 所属场景相同
        }

        bool operator!=(const Object& other)
        {
            return !(*this == other);
        }
    };
}