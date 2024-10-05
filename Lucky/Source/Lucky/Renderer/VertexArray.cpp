#include "lcpch.h"
#include "VertexArray.h"

#include "Renderer.h"

#include <glad/glad.h>

namespace Lucky
{
    /// <summary>
    /// ShaderDataType 转换为 OpenGL 基本类型
    /// </summary>
    /// <param name="">ShaderDataType</param>
    /// <returns>OpenGL 基本类型</returns>
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
        glCreateVertexArrays(1, &m_RendererID); // 创建顶点数组
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_RendererID); // 删除顶点数组
    }

    void VertexArray::Bind() const
    {
        glBindVertexArray(m_RendererID);        // 绑定顶点数组
    }

    void VertexArray::Unbind() const
    {
        glBindVertexArray(0);                   // 解除绑定
    }

    void VertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
    {
        LC_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!"); // 布局元素为零

        glBindVertexArray(m_RendererID);    // 绑定顶点数组
        vertexBuffer->Bind();               // 绑定 VertexBuffer

        // 设置顶点缓冲区布局
        const auto& layout = vertexBuffer->GetLayout();         // 顶点缓冲区布局

        uint32_t index = 0;
        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(index);                   // 启用顶点属性

            glVertexAttribPointer(index,                        // 顶点属性位置编号
                element.GetComponentCount(),                    // 顶点属性数据个数
                ShaderDataTypeToOpenGLBaseType(element.Type),   // 数据类型
                element.Normalized ? GL_TRUE : GL_FALSE,        // 是否标准化
                layout.GetStride(),                             // 顶点大小（字节）
                (const void*)element.Offset);                   // 顶点属性偏移量（字节）

            index++;
        }

        m_VertexBuffers.push_back(vertexBuffer);    // 添加 VertexBuffer 到列表
    }

    void VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
    {
        glBindVertexArray(m_RendererID);    // 绑定顶点数组

        indexBuffer->Bind();                // 绑定 IndexBuffer
        m_IndexBuffer = indexBuffer;        // 设置 IndexBuffer
    }
}