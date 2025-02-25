#pragma once

#include "Lucky/Renderer/VertexArray.h"

#include <glm/glm.hpp>

namespace Lucky
{
    /// <summary>
    /// 渲染命令：渲染接口中最基本的渲染调用
    /// </summary>
    class RenderCommand
    {
    public:
        /// <summary>
        /// 初始化渲染器
        /// </summary>
        static void Init();

        /// <summary>
        /// 设置清屏颜色
        /// </summary>
        /// <param name="color">清屏颜色</param>
        static void SetClearColor(const glm::vec4& color);

        /// <summary>
        /// 设置视口大小
        /// </summary>
        /// <param name="x">左下 x</param>
        /// <param name="y">左下 y</param>
        /// <param name="width">宽</param>
        /// <param name="height">高</param>
        static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

        /// <summary>
        /// 清屏
        /// </summary>
        static void Clear();

        /// <summary>
        /// 绘制索引缓冲区
        /// </summary>
        /// <param name="vertexArray">待绘制的顶点数组</param>
        /// <param name="indexCount">索引个数</param>
        static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0);

        /// <summary>
        /// 绘制直线
        /// </summary>
        /// <param name="vertexArray">待绘制顶点数组</param>
        /// <param name="vertexCount">直线顶点个数</param>
        static void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount);

        /// <summary>
        /// 设置线宽
        /// </summary>
        /// <param name="width">宽度</param>
        static void SetLineWidth(float width);
    };
}