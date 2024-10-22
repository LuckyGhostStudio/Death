#pragma once

struct GLFWwindow;

namespace Lucky
{
    /// <summary>
    /// OpenGL ������
    /// </summary>
    class OpenGLContext
    {
    private:
        GLFWwindow* m_WindowHandle; // GLFW ���ھ��
    public:
        /// <summary>
        /// ���� OpenGL ������
        /// </summary>
        /// <param name="windowHandle">���ھ��</param>
        /// <returns></returns>
        static Scope<OpenGLContext> Create(GLFWwindow* windowHandle);

        OpenGLContext(GLFWwindow* windowHandle);

        /// <summary>
        /// ��ʼ��
        /// </summary>
        void Init();

        /// <summary>
        /// ����ǰ�󻺳�
        /// </summary>
        void SwapBuffers();
    };
}