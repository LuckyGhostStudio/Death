#include "lcpch.h"
#include "RenderCommand.h"

#include <glad/glad.h>

namespace Lucky
{
    void RenderCommand::Init()
    {
        glEnable(GL_BLEND);                                 // 启用颜色混合 TODO 透明度存在问题
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // 最终颜色 = src * alpha + des * (1 - alpha)

        glEnable(GL_DEPTH_TEST);                            // 启用深度测试

        glEnable(GL_LINE_SMOOTH);   // 平滑直线
    }

    void RenderCommand::SetClearColor(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);   // 设置清屏颜色
    }

    void RenderCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(x, y, width, height);    // 设置视口大小
    }

    void RenderCommand::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // 清除 颜色缓冲区 | 深度缓冲区
    }

    void RenderCommand::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
    {
        vertexArray->Bind();
        uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();

        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);  // 顶点数组索引绘制三角形
    }

    void RenderCommand::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
    {
        vertexArray->Bind();

        glDrawArrays(GL_LINES, 0, vertexCount); // 绘制直线
    }

    void RenderCommand::SetLineWidth(float width)
    {
        glLineWidth(width);
    }
}