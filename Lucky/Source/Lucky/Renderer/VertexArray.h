#pragma once

#include <memory>
#include "Lucky/Renderer/Buffer.h"

namespace Lucky
{
    /// <summary>
    /// 顶点数组
    /// </summary>
    class VertexArray
    {
    private:
        uint32_t m_RendererID;                                      // 顶点数组 ID
        std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers; // 绑定在顶点数组的 VertexBuffer 列表
        std::shared_ptr<IndexBuffer> m_IndexBuffer;                 // 绑定在顶点数组的 IndexBuffer
    public:
        VertexArray();

        ~VertexArray();

        /// <summary>
        /// 绑定
        /// </summary>
        void Bind() const;

        /// <summary>
        /// 解除绑定
        /// </summary>
        void Unbind() const;

        /// <summary>
        /// 添加顶点缓冲
        /// </summary>
        /// <param name="vertexBuffer">顶点缓冲</param>
        void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);

        /// <summary>
        /// 设置顶点索引缓冲
        /// </summary>
        /// <param name="indexBuffer">索引缓冲</param>
        void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

        /// <summary>
        /// 返回 VertexBuffer 列表
        /// </summary>
        /// <returns>顶点缓冲列表</returns>
        const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }

        /// <summary>
        /// 返回 IndexBuffer
        /// </summary>
        /// <returns>索引缓冲</returns>
        const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
    };
}