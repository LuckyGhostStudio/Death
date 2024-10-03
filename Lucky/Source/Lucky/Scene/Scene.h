#pragma once

#include "entt.hpp"

#include "Lucky/Core/DeltaTime.h"

namespace Lucky
{
    class Object;

    /// <summary>
    /// ����
    /// </summary>
    class Scene
    {
    private:
        friend class Object;        // ��Ԫ�� Object

        entt::registry m_Registry;  // ʵ�弯�ϣ�ʵ�� id ���ϣ�unsigned int ���ϣ�
        std::string m_Name;         // ������
    public:
        Scene(const std::string& name = "New Scene");
        ~Scene();

        inline const std::string& GetName() const { return m_Name; }
        inline void SetName(const std::string& name) { m_Name = name; }

        /// <summary>
        /// ��������
        /// </summary>
        /// <param name="name">������</param>
        /// <returns>����</returns>
        Object CreateObject(const std::string& name = "Object");

        /// <summary>
        /// ���£�ÿ֡����
        /// </summary>
        /// <param name="dt">֡���</param>
        void OnUpdate(DeltaTime dt);
    };
}