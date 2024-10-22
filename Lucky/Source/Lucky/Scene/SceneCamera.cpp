#include "lcpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Lucky
{
    SceneCamera::SceneCamera()
    {
        RecalculateProjection();    // ���¼���ͶӰ����
    }

    void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
    {
        m_ProjectionType = ProjectionType::Orthographic;    // ����ͶӰ

        m_Size = size;
        m_Near = nearClip;
        m_Far = farClip;

        RecalculateProjection();    // ���¼���ͶӰ����
    }

    void SceneCamera::SetPerspective(float fov, float nearClip, float farClip)
    {
        m_ProjectionType = ProjectionType::Perspective;     //͸��ͶӰ

        m_Fov = glm::radians(fov);
        m_Near = nearClip;
        m_Far = farClip;

        RecalculateProjection();    // ���¼���ͶӰ����
    }

    void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
    {
        LC_CORE_ASSERT(width > 0 && height > 0, "width or height <= 0!");

        m_AspectRatio = (float)width / (float)height;   // ��߱�

        RecalculateProjection();    // ���¼���ͶӰ����
    }

    void SceneCamera::RecalculateProjection()
    {
        if (m_ProjectionType == ProjectionType::Perspective)    // ͸��ͶӰ
        {
            m_ProjectionMatrix = glm::perspective(m_Fov, m_AspectRatio, m_Near, m_Far); // ����͸��ͶӰ����
        }
        else
        {
            float orthLeft = -m_Size * m_AspectRatio;   // ��߽�
            float orthRight = m_Size * m_AspectRatio;   // �ұ߽�
            float orthBottom = -m_Size;                 // �±߽�
            float orthTop = m_Size;                     // �ϱ߽�

            m_ProjectionMatrix = glm::ortho(orthLeft, orthRight, orthBottom, orthTop, m_Near, m_Far);   // ��������ͶӰ����
        }
    }
}