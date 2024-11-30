#include "lcpch.h"
#include "Input.h"

#include "Lucky/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Lucky
{
    bool Input::IsKeyPressed(KeyCode keycode)
    {
        auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());   // GLFW ����
        auto state = glfwGetKey(window, (int)keycode);  // ��ȡ keycode ����״̬
        return state == GLFW_PRESS;                     // ��������
    }
    bool Input::IsMouseButtonPressed(MouseCode button)
    {
        auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());
        auto state = glfwGetMouseButton(window, (int)button);   // ��ȡ button ״̬
        return state == GLFW_PRESS;                             // ��������
    }

    std::pair<float, float> Input::GetMousePosition()
    {
        auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());

        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos); // ��ȡ��� x y ����

        return { (float)xPos, (float)yPos };
    }

    float Input::GetMouseX()
    {
        return GetMousePosition().first;    // x ����
    }
    float Input::GetMouseY()
    {
        return GetMousePosition().second;   // y ����
    }
}