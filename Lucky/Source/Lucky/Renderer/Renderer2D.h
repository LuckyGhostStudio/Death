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
        /// ��ʼ����Ⱦ��
        /// </summary>
        static void Init();

        static void Shutdown();

        /// <summary>
        /// ��ʼ��Ⱦ����
        /// </summary>
        /// <param name="camera">�༭�����</param>
        static void BeginScene(const EditorCamera& camera);

        /// <summary>
        /// ��ʼ��Ⱦ���������ó�������
        /// </summary>
        /// <param name="camera">���</param>
        /// <param name="transform">Transform</param>
        static void BeginScene(const Camera& camera, const Transform& transform);

        /// <summary>
        /// ������Ⱦ����
        /// </summary>
        static void EndScene();

        /// <summary>
        /// ˢ�£���������Ⱦ����
        /// </summary>
        static void Flush();

        /// <summary>
        /// ���Ƴ�����
        /// </summary>
        /// <param name="transform">Transform</param>
        /// <param name="color">��ɫ</param>
        /// <param name="objectID">���� ID</param>
        static void DrawQuad(const Transform& transform, const glm::vec4& color, int objectID = -1);

        /// <summary>
        /// ���Ƴ�����
        /// </summary>
        /// <param name="transform">Transform</param>
        /// <param name="color">��ɫ</param>
        /// <param name="texture">����</param>
        /// <param name="objectID">���� ID</param>
        static void DrawQuad(const Transform& transform, const glm::vec4& color, Ref<Texture2D>& texture, int objectID = -1);

        /// <summary>
        /// ����ֱ�� TODO LineRenderer in Component
        /// </summary>
        /// <param name="p0">���</param>
        /// <param name="p1">�յ�</param>
        /// <param name="color">��ɫ</param>
        /// <param name="objectID">���� ID</param>
        static void DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int objectID = -1);

        static void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int objectID = -1);

        static void DrawRect(const Transform& transform, const glm::vec4& color, int objectID = -1);

        static float GetLineWidth();
        static void SetLineWidth(float width);

        /// <summary>
        /// ͳ������
        /// </summary>
        struct Statistics
        {
            uint32_t DrawCalls = 0; // ���Ƶ��ô���
            uint32_t QuadCount = 0; // �ı��θ���

            /// <summary>
            /// �����ܶ������
            /// </summary>
            /// <returns></returns>
            uint32_t GetTotalVertexCount() { return QuadCount * 4; }

            /// <summary>
            /// ��������������
            /// </summary>
            /// <returns></returns>
            uint32_t GetTotalIndexCount() { return QuadCount * 6; }
        };

        static Statistics GetStats();

        /// <summary>
        /// ����ͳ������
        /// </summary>
        static void ResetStats();
    private:
        /// <summary>
        /// ��ʼ����Ⱦ����ʼ������Ⱦ����
        /// </summary>
        static void StartBatch();

        /// <summary>
        /// ��һ������Ⱦ
        /// </summary>
        static void NextBatch();
    };
}