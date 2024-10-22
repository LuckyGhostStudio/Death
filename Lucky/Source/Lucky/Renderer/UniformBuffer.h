#pragma once

#include "Lucky/Core/Core.h"

namespace Lucky
{
    /// <summary>
    /// Uniform ������
    /// </summary>
    class UniformBuffer
    {
    private:
        uint32_t m_RendererID = 0;
    public:
        /// <summary>
        /// ���� Uniform ������
        /// </summary>
        /// <param name="size">��С���ֽڣ�</param>
        /// <param name="binding">�󶨵�����</param>
        /// <returns></returns>
        static Ref<UniformBuffer> Create(uint32_t size, uint32_t binding);
        
        UniformBuffer(uint32_t size, uint32_t binding);

        ~UniformBuffer();

        /// <summary>
        /// ��������
        /// </summary>
        /// <param name="data">����</param>
        /// <param name="size">��С���ֽڣ�</param>
        /// <param name="offset">ƫ����</param>
        void SetData(const void* data, uint32_t size, uint32_t offset = 0);
    };
}