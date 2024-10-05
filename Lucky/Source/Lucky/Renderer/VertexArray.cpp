#include "lcpch.h"
#include "VertexArray.h"

#include "Renderer.h"

#include <glad/glad.h>

namespace Lucky
{
    /// <summary>
    /// ShaderDataType ת��Ϊ OpenGL ��������
    /// </summary>
    /// <param name="">ShaderDataType</param>
    /// <returns>OpenGL ��������</returns>
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float:     return GL_FLOAT;
            case ShaderDataType::Float2:    return GL_FLOAT;
            case ShaderDataType::Float3:    return GL_FLOAT;
            case ShaderDataType::Float4:    return GL_FLOAT;
            case ShaderDataType::Mat3:      return GL_FLOAT;
            case ShaderDataType::Mat4:      return GL_FLOAT;
            case ShaderDataType::Int:       return GL_INT;
            case ShaderDataType::Int2:      return GL_INT;
            case ShaderDataType::Int3:      return GL_INT;
            case ShaderDataType::Int4:      return GL_INT;
            case ShaderDataType::Bool:      return GL_BOOL;
        }

        LC_CORE_ASSERT(false, "Unknow ShaderDataType!");

        return 0;
    }

    VertexArray::VertexArray()
    {
        glCreateVertexArrays(1, &m_RendererID); // ������������
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_RendererID); // ɾ����������
    }

    void VertexArray::Bind() const
    {
        glBindVertexArray(m_RendererID);        // �󶨶�������
    }

    void VertexArray::Unbind() const
    {
        glBindVertexArray(0);                   // �����
    }

    void VertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
    {
        LC_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!"); // ����Ԫ��Ϊ��

        glBindVertexArray(m_RendererID);    // �󶨶�������
        vertexBuffer->Bind();               // �� VertexBuffer

        // ���ö��㻺��������
        const auto& layout = vertexBuffer->GetLayout();         // ���㻺��������

        uint32_t index = 0;
        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(index);                   // ���ö�������

            glVertexAttribPointer(index,                        // ��������λ�ñ��
                element.GetComponentCount(),                    // �����������ݸ���
                ShaderDataTypeToOpenGLBaseType(element.Type),   // ��������
                element.Normalized ? GL_TRUE : GL_FALSE,        // �Ƿ��׼��
                layout.GetStride(),                             // �����С���ֽڣ�
                (const void*)element.Offset);                   // ��������ƫ�������ֽڣ�

            index++;
        }

        m_VertexBuffers.push_back(vertexBuffer);    // ��� VertexBuffer ���б�
    }

    void VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
    {
        glBindVertexArray(m_RendererID);    // �󶨶�������

        indexBuffer->Bind();                // �� IndexBuffer
        m_IndexBuffer = indexBuffer;        // ���� IndexBuffer
    }
}