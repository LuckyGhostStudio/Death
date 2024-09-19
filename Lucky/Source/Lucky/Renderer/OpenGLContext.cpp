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
		glfwMakeContextCurrent(m_WindowHandle);					// ���ô���������Ϊ��ǰ�߳���������

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);	// ��ʼ��GLAD
		LC_CORE_ASSERT(status, "Failed to initialize Glad!");

		LC_CORE_INFO("OpenGL Info");										// OpenGL��Ϣ
		LC_CORE_INFO("    Vendor: {0}", (char*)glGetString(GL_VENDOR));		// ������
		LC_CORE_INFO("    Renderer: {0}", (char*)glGetString(GL_RENDERER));	// GPU����
		LC_CORE_INFO("    Version: {0}", (char*)glGetString(GL_VERSION));	// �汾
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);	// ����ǰ�󻺳���
	}
}