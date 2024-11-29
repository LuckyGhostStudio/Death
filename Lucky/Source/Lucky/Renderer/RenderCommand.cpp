#include "lcpch.h"
#include "RenderCommand.h"

#include <glad/glad.h>

namespace Lucky
{
    void RenderCommand::Init()
    {
        glEnable(GL_BLEND);                                 // ������ɫ��� TODO ͸���ȴ�������
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // ������ɫ = src * alpha + des * (1 - alpha)

        glEnable(GL_DEPTH_TEST);                            // ������Ȳ���

        glEnable(GL_LINE_SMOOTH);   // ƽ��ֱ��
    }

    void RenderCommand::SetClearColor(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);   // ����������ɫ
    }

    void RenderCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(x, y, width, height);    // �����ӿڴ�С
    }

    void RenderCommand::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // ��� ��ɫ������ | ��Ȼ�����
    }

    void RenderCommand::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
    {
        vertexArray->Bind();
        uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();

        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);  // ����������������������
    }

    void RenderCommand::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
    {
        vertexArray->Bind();

        glDrawArrays(GL_LINES, 0, vertexCount); // ����ֱ��
    }

    void RenderCommand::SetLineWidth(float width)
    {
        glLineWidth(width);
    }
}