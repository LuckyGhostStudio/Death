#pragma once

#include "Camera.h"
#include "Transform.h"
#include "Texture.h"
#include "EditorCamera.h"

namespace Lucky
{
    class Renderer2D
    {
    public:
        /// <summary>
        /// 初始化渲染器
        /// </summary>
        static void Init();

        static void Shutdown();

        /// <summary>
        /// 开始渲染场景
        /// </summary>
        /// <param name="camera">编辑器相机</param>
        static void BeginScene(const EditorCamera& camera);

        /// <summary>
        /// 开始渲染场景：设置场景参数
        /// </summary>
        /// <param name="camera">相机</param>
        /// <param name="transform">Transform</param>
        static void BeginScene(const Camera& camera, const Transform& transform);

        /// <summary>
        /// 结束渲染场景
        /// </summary>
        static void EndScene();

        /// <summary>
        /// 刷新：绘制批渲染数据
        /// </summary>
        static void Flush();

        /// <summary>
        /// 绘制长方形
        /// </summary>
        /// <param name="transform">Transform</param>
        /// <param name="color">颜色</param>
        /// <param name="objectID">物体 ID</param>
        static void DrawQuad(const Transform& transform, const glm::vec4& color, int objectID = -1);

        /// <summary>
        /// 绘制长方形
        /// </summary>
        /// <param name="transform">Transform</param>
        /// <param name="color">颜色</param>
        /// <param name="texture">纹理</param>
        /// <param name="objectID">物体 ID</param>
        static void DrawQuad(const Transform& transform, const glm::vec4& color, Ref<Texture2D>& texture, int objectID = -1);
    
        /// <summary>
        /// 统计数据
        /// </summary>
        struct Statistics
        {
            uint32_t DrawCalls = 0; // 绘制调用次数
            uint32_t QuadCount = 0; // 四边形个数

            /// <summary>
            /// 返回总顶点个数
            /// </summary>
            /// <returns></returns>
            uint32_t GetTotalVertexCount() { return QuadCount * 4; }

            /// <summary>
            /// 返回总索引个数
            /// </summary>
            /// <returns></returns>
            uint32_t GetTotalIndexCount() { return QuadCount * 6; }
        };

        static Statistics GetStats();

        /// <summary>
        /// 重置统计数据
        /// </summary>
        static void ResetStats();
    private:
        /// <summary>
        /// 开始批渲染：初始化批渲染数据
        /// </summary>
        static void StartBatch();

        /// <summary>
        /// 下一次批渲染
        /// </summary>
        static void NextBatch();
    };
}