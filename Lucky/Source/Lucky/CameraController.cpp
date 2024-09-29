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
        // ����ƶ�
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

            m_Camera.SetRotation(m_CameraRotation); // ���������ת
        }

        m_Camera.SetPosition(m_CameraPosition);     // �������λ��

        m_CameraTranslationSpeed = m_ZoomLevel;     // �ƶ��ٶȺ����ű����������
    }

    void CameraController::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);

        dispatcher.Dispatch<MouseScrolledEvent>(LC_BIND_EVENT_FUNC(CameraController::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(LC_BIND_EVENT_FUNC(CameraController::OnWindowResized));
    }

    bool CameraController::OnMouseScrolled(MouseScrolledEvent& e)
    {
        m_ZoomLevel -= e.GetYOffset() * 0.5f;       // Y �����ű�����С
        m_ZoomLevel = std::max(m_ZoomLevel, 0.25f); // ���� >= 0.25
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);   // ����ͶӰ����

        return false;
    }

    bool CameraController::OnWindowResized(WindowResizeEvent& e)
    {
        m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();                                                     // ��߱�Ϊ ���� ��߱�
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);   // ����ͶӰ����

        return false;
    }
}