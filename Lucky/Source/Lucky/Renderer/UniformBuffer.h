#pragma once

#include "Lucky/Core/Core.h"

namespace Lucky
{
    /// <summary>
    /// Uniform 缓冲区
    /// </summary>
    class UniformBuffer
    {
    private:
        uint32_t m_RendererID = 0;
    public:
        /// <summary>
        /// 创建 Uniform 缓冲区
        /// </summary>
        /// <param name="size">大小（字节）</param>
        /// <param name="binding">绑定点索引</param>
        /// <returns></returns>
        static Ref<UniformBuffer> Create(uint32_t size, uint32_t binding);
        
        UniformBuffer(uint32_t size, uint32_t binding);

        ~UniformBuffer();

        /// <summary>
        /// 设置数据
        /// </summary>
        /// <param name="data">数据</param>
        /// <param name="size">大小（字节）</param>
        /// <param name="offset">偏移量</param>
        void SetData(const void* data, uint32_t size, uint32_t offset = 0);
    };
}