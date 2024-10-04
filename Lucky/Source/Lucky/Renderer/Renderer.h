#pragma once

#include "RenderCommand.h"

#include "Camera.h"

namespace Lucky
{
    /// <summary>
    /// ��Ⱦ��
    /// </summary>
    class Renderer
    {
    public:
        /// <summary>
        /// ��ʼ����Ⱦ��
        /// </summary>
        static void Init();

        /// <summary>
        /// ��������ʱ����
        /// </summary>
        /// <param name="width">��</param>
        /// <param name="height">��</param>
        static void OnWindowResize(uint32_t width, uint32_t height);
    };
}