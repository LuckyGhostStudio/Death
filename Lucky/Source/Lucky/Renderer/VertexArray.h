#pragma once

#include <memory>
#include "Lucky/Renderer/Buffer.h"

namespace Lucky
{
    /// <summary>
    /// ��������
    /// </summary>
    class VertexArray
    {
    private:
        uint32_t m_RendererID;                                      // �������� ID
        std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers; // ���ڶ�������� VertexBuffer �б�
        std::shared_ptr<IndexBuffer> m_IndexBuffer;                 // ���ڶ�������� IndexBuffer
    public:
        VertexArray();

        ~VertexArray();

        /// <summary>
        /// ��
        /// </summary>
        void Bind() const;

        /// <summary>
        /// �����
        /// </summary>
        void Unbind() const;

        /// <summary>
        /// ��Ӷ��㻺��
        /// </summary>
        /// <param name="vertexBuffer">���㻺��</param>
        void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);

        /// <summary>
        /// ���ö�����������
        /// </summary>
        /// <param name="indexBuffer">��������</param>
        void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

        /// <summary>
        /// ���� VertexBuffer �б�
        /// </summary>
        /// <returns>���㻺���б�</returns>
        const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }

        /// <summary>
        /// ���� IndexBuffer
        /// </summary>
        /// <returns>��������</returns>
        const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
    };
}