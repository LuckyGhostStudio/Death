#include "lcpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Lucky
{
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
    {
        LC_CORE_ASSERT(m_WindowHandle, "Window handle is null!");
    }

    void OpenGLContext::Init()
    {
        glfwMakeContextCurrent(m_WindowHandle); // 设置窗口上下文为当前线程主上下文

        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);    // 初始化 GLAD
        LC_CORE_ASSERT(status, "Failed to initialize Glad!");

        LC_CORE_INFO("OpenGL Info");                                        // OpenGL 信息
        LC_CORE_INFO("    Vendor: {0}", (char*)glGetString(GL_VENDOR));     // 出版商
        LC_CORE_INFO("    Renderer: {0}", (char*)glGetString(GL_RENDERER)); // GPU 类型
        LC_CORE_INFO("    Version: {0}", (char*)glGetString(GL_VERSION));   // 版本

        // 检查 OpenGL 版本
        LC_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Lucky requires at least OpenGL version 4.5!");
    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);    // 交换前后缓冲区
    }
}