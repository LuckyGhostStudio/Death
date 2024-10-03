#pragma once

#include "entt.hpp"

#include "Lucky/Core/DeltaTime.h"

namespace Lucky
{
    class Object;

    /// <summary>
    /// 场景
    /// </summary>
    class Scene
    {
    private:
        friend class Object;        // 友元类 Object

        entt::registry m_Registry;  // 实体集合：实体 id 集合（unsigned int 集合）
        std::string m_Name;         // 场景名
    public:
        Scene(const std::string& name = "New Scene");
        ~Scene();

        inline const std::string& GetName() const { return m_Name; }
        inline void SetName(const std::string& name) { m_Name = name; }

        /// <summary>
        /// 创建物体
        /// </summary>
        /// <param name="name">物体名</param>
        /// <returns>物体</returns>
        Object CreateObject(const std::string& name = "Object");

        /// <summary>
        /// 更新：每帧调用
        /// </summary>
        /// <param name="dt">帧间隔</param>
        void OnUpdate(DeltaTime dt);
    };
}