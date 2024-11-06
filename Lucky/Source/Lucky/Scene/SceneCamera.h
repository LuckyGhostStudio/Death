#pragma once

#include "Lucky/Renderer/Camera.h"

#include <glm/glm.hpp>

namespace Lucky
{
    /// <summary>
    /// 场景相机
    /// </summary>
    class SceneCamera : public Camera
    {
    public:
        /// <summary>
        /// 清屏标志
        /// </summary>
        enum class ClearFlag
        {
            Color = 0,      // 颜色
            Skybox = 1      // 天空盒
        };

        /// <summary>
        /// 投影类型
        /// </summary>
        enum class ProjectionType
        {
            Perspective = 0,    // 透视投影
            Orthographic = 1    // 正交投影
        };
    private:
        bool m_Primary = true;              // 是否是主相机

        glm::vec4 m_BackgroundColor = { 0.2f, 0.3f, 0.5f, 1.0f };       // 清屏颜色

        ClearFlag m_ClearFlag = ClearFlag::Color;                       // 清屏类型
        ProjectionType m_ProjectionType = ProjectionType::Perspective;  // 投影类型

        float m_Fov = glm::radians(45.0f);  // 透视相机张角：垂直方向（弧度）
        float m_Size = 5.0f;                // 正交相机尺寸：垂直方向一半长度

        float m_Near = 0.01f;               // 近裁剪平面
        float m_Far = 1000.0f;              // 远裁剪平面

        float m_AspectRatio = 0.0f;         // 屏幕宽高比（X/Y）

        /// <summary>
        /// 重新计算投影矩阵
        /// </summary>
        void RecalculateProjection();
    public:
        SceneCamera();

        /// <summary>
        /// 设置透视投影参数->计算投影矩阵
        /// </summary>
        /// <param name="fov">垂直张角大小</param>
        /// <param name="nearClip">近裁剪平面</param>
        /// <param name="farClip">远裁剪平面</param>
        void SetPerspective(float fov, float nearClip, float farClip);

        /// <summary>
        /// 设置正交投影参数->计算投影矩阵
        /// </summary>
        /// <param name="size">尺寸：垂直方向</param>
        /// <param name="nearClip">近裁剪平面</param>
        /// <param name="farClip">远裁剪平面</param>
        void SetOrthographic(float size, float nearClip, float farClip);

        /// <summary>
        /// 设置视口大小->计算投影矩阵
        /// </summary>
        /// <param name="width">宽</param>
        /// <param name="height">高</param>
        void SetViewportSize(uint32_t width, uint32_t height);

        bool IsPrimary() const { return m_Primary; }
        bool& IsPrimary_Ref() { return m_Primary; }
        void SetPrimary(bool primary) { m_Primary = primary; }

        const glm::vec4& GetBackgroundColor() const { return m_BackgroundColor; }
        void SetBackgroundColor(const glm::vec4& background) { m_BackgroundColor = background; }

        ClearFlag GetClearFlag() const { return m_ClearFlag; }
        void SetClearFlag(ClearFlag clearFlag) { m_ClearFlag = clearFlag; }

        ProjectionType GetProjectionType() const { return m_ProjectionType; }
        void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }

        float GetFOV() const { return m_Fov; }
        float& GetFOV_Ref() { return m_Fov; }
        void SetFOV(float fov) { m_Fov = fov; RecalculateProjection(); }

        float GetSize() const { return m_Size; }
        float& GetSize_Ref() { return m_Size; }
        void SetSize(float size) { m_Size = size; RecalculateProjection(); }

        float GetNearClip() const { return m_Near; }
        float& GetNearClip_Ref() { return m_Near; }
        void SetNearClip(float nearClip) { m_Near = nearClip; RecalculateProjection(); }

        float GetFarClip() const { return m_Far; }
        float& GetFarClip_Ref() { return m_Far; }
        void SetFarClip(float farClip) { m_Far = farClip; RecalculateProjection(); }
    };
}