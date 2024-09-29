#include "lcpch.h"
#include "CameraController.h"

#include "Lucky/Input/Input.h"
#include "Lucky/Input/KeyCodes.h"

namespace Lucky
{
    CameraController::CameraController(float aspectRatio, bool rotation)
        :m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
    {

    }

    void CameraController::OnUpdate(DeltaTime dt)
    {
        // 相机移动
        if (Input::IsKeyPressed(Key::Left)) {
            m_CameraPosition.x -= m_CameraTranslationSpeed * dt;
        }
        else if (Input::IsKeyPressed(Key::Right)) {
            m_CameraPosition.x += m_CameraTranslationSpeed * dt;
        }
        if (Input::IsKeyPressed(Key::Up)) {
            m_CameraPosition.y += m_CameraTranslationSpeed * dt;
        }
        else if (Input::IsKeyPressed(Key::Down)) {
            m_CameraPosition.y -= m_CameraTranslationSpeed * dt;
        }

        if (m_Rotation) {
            if (Input::IsKeyPressed(Key::Q)) {
                m_CameraRotation += m_CameraRotationSpeed * dt;
            }
            if (Input::IsKeyPressed(Key::E)) {
                m_CameraRotation -= m_CameraRotationSpeed * dt;
            }

            m_Camera.SetRotation(m_CameraRotation); // 设置相机旋转
        }

        m_Camera.SetPosition(m_CameraPosition);     // 设置相机位置

        m_CameraTranslationSpeed = m_ZoomLevel;     // 移动速度和缩放比例线性相关
    }

    void CameraController::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);

        dispatcher.Dispatch<MouseScrolledEvent>(LC_BIND_EVENT_FUNC(CameraController::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(LC_BIND_EVENT_FUNC(CameraController::OnWindowResized));
    }

    bool CameraController::OnMouseScrolled(MouseScrolledEvent& e)
    {
        m_ZoomLevel -= e.GetYOffset() * 0.5f;       // Y 轴缩放比例减小
        m_ZoomLevel = std::max(m_ZoomLevel, 0.25f); // 缩放 >= 0.25
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);   // 设置投影矩阵

        return false;
    }

    bool CameraController::OnWindowResized(WindowResizeEvent& e)
    {
        m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();                                                     // 宽高比为 窗口 宽高比
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);   // 设置投影矩阵

        return false;
    }
}