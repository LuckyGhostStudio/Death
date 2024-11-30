#include "lcpch.h"
#include "Input.h"

#include "Lucky/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Lucky
{
    bool Input::IsKeyPressed(KeyCode keycode)
    {
        auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());   // GLFW 窗口
        auto state = glfwGetKey(window, (int)keycode);  // 获取 keycode 按键状态
        return state == GLFW_PRESS;                     // 按键按下
    }
    bool Input::IsMouseButtonPressed(MouseCode button)
    {
        auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());
        auto state = glfwGetMouseButton(window, (int)button);   // 获取 button 状态
        return state == GLFW_PRESS;                             // 按键按下
    }

    std::pair<float, float> Input::GetMousePosition()
    {
        auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());

        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos); // 获取鼠标 x y 坐标

        return { (float)xPos, (float)yPos };
    }

    float Input::GetMouseX()
    {
        return GetMousePosition().first;    // x 坐标
    }
    float Input::GetMouseY()
    {
        return GetMousePosition().second;   // y 坐标
    }
}