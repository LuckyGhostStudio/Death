#pragma once

#include "entt.hpp"

#include "Lucky/Core/DeltaTime.h"
#include "Lucky/Renderer/EditorCamera.h"

class b2World;

namespace Lucky
{
    class Object;

    /// <summary>
    /// 场景
    /// </summary>
    class Scene
    {
    private:
        friend class Object;                // 友元类 Object
        friend class SceneHierarchyPanel;   // 友元类 SceneHierarchyPanel
        friend class SceneSerializer;       // 友元类 SceneSerializer

        b2World* m_PhysicsWorld = nullptr;

        entt::registry m_Registry;          // 实体集合：实体 id 集合（unsigned int 集合）
        std::string m_Name;                 // 场景名

        uint32_t m_ViewportWidth = 1280;    // 场景视口宽
        uint32_t m_ViewportHeight = 720;    // 场景视口高
    private:
        /// <summary>
        /// object 添加 T 组件时调用
        /// </summary>
        /// <typeparam name="T">组件类型</typeparam>
        /// <param name="object">物体</param>
        /// <param name="component">组件</param>
        template<typename T>
        void OnComponentAdded(Object object, T& component);
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
        /// 删除物体
        /// </summary>
        /// <param name="object">物体</param>
        void DeleteObject(Object object);

        void OnRuntimeStart();

        void OnRuntimeStop();

        /// <summary>
        /// 编辑器更新：每帧调用
        /// </summary>
        /// <param name="dt">帧间隔</param>
        /// <param name="camera">编辑器相机</param>
        void OnUpdateEditor(DeltaTime dt, EditorCamera& camera);

        /// <summary>
        /// 运行时更新：每帧调用
        /// </summary>
        /// <param name="dt">帧间隔</param>
        void OnUpdateRuntime(DeltaTime dt);

        /// <summary>
        /// 重置视口大小：视口改变时调用
        /// </summary>
        /// <param name="width">宽</param>
        /// <param name="height">高</param>
        void OnViewportResize(uint32_t width, uint32_t height);

        /// <summary>
        /// 返回主相机
        /// </summary>
        /// <returns>主相机</returns>
        Object GetPrimaryCameraObject();
    };
}