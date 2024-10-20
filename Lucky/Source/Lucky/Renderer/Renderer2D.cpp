#include "lcpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/ext/matrix_transform.hpp>
#include <glad/glad.h>

namespace Lucky
{
    /// <summary>
    /// 四边形顶点
    /// </summary>
    struct QuadVertex
    {
        glm::vec3 Position; // 位置
        glm::vec4 Color;    // 颜色
        glm::vec2 TexCoord; // 纹理坐标
        float TexIndex;     // 纹理索引
        int ObjectID;       // 物体 ID
    };

    /// <summary>
    /// 2D 渲染器数据
    /// </summary>
    struct Renderer2DData
    {
        static const uint32_t MaxQuads = 10000;             // 最大四边形数量
        static const uint32_t MaxVertices = MaxQuads * 4;   // 最大顶点数
        static const uint32_t MaxIndices = MaxQuads * 6;    // 最大索引数（4 个顶点 6 个索引）
        static const uint32_t MaxTextureSlots = 32;         // 最大纹理槽数

        Ref<VertexArray> QuadVertexArray;   // 四边形顶点数组
        Ref<VertexBuffer> QuadVertexBuffer; // 四边形顶点缓冲区
        Ref<Shader> TextureShader;          // 纹理着色器
        Ref<Texture2D>    WhiteTexture;     // 白色纹理

        uint32_t QuadIndexCount = 0;                    // 四边形索引个数
        QuadVertex* QuadVertexBufferBase = nullptr;     // 顶点数据
        QuadVertex* QuadVertexBufferPtr = nullptr;      // 顶点数据指针

        std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;   // 纹理槽列表 存储纹理
        uint32_t TextureSlotIndex = 1;                              // 纹理槽索引 0 = White

        glm::vec4 QuadVerticesPositions[4];     // 顶点位置
        Renderer2D::Statistics Stats;           // 统计数据
    };

    static Renderer2DData s_Data;   // 渲染器数据

    void Renderer2D::Init()
    {
        s_Data.QuadVertexArray = CreateRef<VertexArray>();  // 创建顶点数组对象

        s_Data.QuadVertexBuffer = CreateRef<VertexBuffer>(s_Data.MaxVertices * sizeof(QuadVertex)); // 创建顶点缓冲

        // 设置顶点缓冲区布局
        s_Data.QuadVertexBuffer->SetLayout(
        {
            { ShaderDataType::Float3, "a_Position" },   // 位置
            { ShaderDataType::Float4, "a_Color" },      // 颜色
            { ShaderDataType::Float2, "a_Texture" },    // 纹理坐标
            { ShaderDataType::Float, "a_TexIndex" },    // 纹理索引
            { ShaderDataType::Int, "a_ObjectID" },      // 物体 ID
        });

        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);   // 添加 IndexBuffer

        s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];   // 顶点数据

        uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];            // 顶点索引

        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
        {
            quadIndices[i + 0] = offset + 0;    // 左下
            quadIndices[i + 1] = offset + 1;    // 右下
            quadIndices[i + 2] = offset + 2;    // 右上

            quadIndices[i + 3] = offset + 2;    // 右上
            quadIndices[i + 4] = offset + 3;    // 左上
            quadIndices[i + 5] = offset + 0;    // 左下

            offset += 4;    // 偏移 4 个顶点
        }

        Ref<IndexBuffer> quadIB = CreateRef<IndexBuffer>(quadIndices, s_Data.MaxIndices);   // 创建索引缓冲
        s_Data.QuadVertexArray->SetIndexBuffer(quadIB);                                     // 设置 IndexBuffer
        delete[] quadIndices;

        s_Data.WhiteTexture = CreateRef<Texture2D>(1, 1);                   // 创建宽高为 1 的纹理
        uint32_t whitTextureData = 0xffffffff;                              // 255 白色
        s_Data.WhiteTexture->SetData(&whitTextureData, sizeof(uint32_t));   // 设置纹理数据 size = 1 * 1 * 4 == sizeof(uint32_t)

        // 纹理采样器 0 - 31
        int samplers[s_Data.MaxTextureSlots];
        for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
        {
            samplers[i] = i;
        }

        s_Data.TextureShader = CreateRef<Shader>("Assets/Shaders/TextureShader");   // 创建 Texture 着色器

        s_Data.TextureShader->Bind();                    // 绑定 Texture 着色器

        s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);  // 设置 textures 变量 所有纹理槽
        s_Data.TextureSlots[0] = s_Data.WhiteTexture;    // 0 号纹理槽为白色纹理（默认）

        s_Data.QuadVerticesPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.QuadVerticesPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.QuadVerticesPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
        s_Data.QuadVerticesPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
    }

    void Renderer2D::Shutdown()
    {
        delete[] s_Data.QuadVertexBufferBase;
    }

    void Renderer2D::BeginScene(const EditorCamera& camera)
    {
        s_Data.TextureShader->Bind();   // 绑定 Texture 着色器
        s_Data.TextureShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());  //设置 vp 矩阵

        StartBatch();   // 开始批渲染
    }

    void Renderer2D::BeginScene(const Camera& camera, const Transform& transform)
    {
        glm::mat4 viewProjectMatrix = camera.GetProjectionMatrix() * glm::inverse(transform.GetTransform());    // vp = p * v

        s_Data.TextureShader->Bind();   // 绑定 Texture 着色器
        s_Data.TextureShader->SetMat4("u_ViewProjectionMatrix", viewProjectMatrix);   // 设置 vp 矩阵

        StartBatch();   // 开始批渲染
    }

    void Renderer2D::EndScene()
    {
        Flush();
    }

    void Renderer2D::StartBatch()
    {
        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;   // 初始化顶点数据指针

        s_Data.TextureSlotIndex = 1;    // 纹理槽索引从 1 开始 0 为白色纹理
    }

    void Renderer2D::Flush()
    {
        if (s_Data.QuadIndexCount == 0)
        {
            return;
        }
        
        uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);   // 数据大小（字节）
        s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);    // 设置顶点缓冲区数据

        // 绑定所有纹理
        for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
        {
            s_Data.TextureSlots[i]->Bind(i);    // 绑定 i 号纹理槽
        }

        RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);  // 绘制

        s_Data.Stats.DrawCalls++;   // 绘制调用次数 ++
    }

    void Renderer2D::NextBatch()
    {
        Flush();

        StartBatch();
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const glm::vec4& color)
    {
        //DrawQuad({ position.x, position.y, 0.0f }, rotation, { scale.x, scale.y, 1.0f }, color);
    }

    void Renderer2D::DrawQuad(const Transform& transform, const glm::vec4& color, int objectID)
    {
        // 索引个数超过最大索引数
        if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
        {
            NextBatch();    // 开始新一次批渲染
        }

        constexpr int quadVertexCount = 4;  // 顶点个数
        constexpr glm::vec2 texCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };   // 纹理坐标
        const float texIndex = 0.0f;        // 白色纹理索引

        // Transform 矩阵
        const glm::mat4& transformMat = transform.GetTransform();

        // 4 个顶点数据
        for (int i = 0; i < quadVertexCount; i++)
        {
            s_Data.QuadVertexBufferPtr->Position = transformMat * s_Data.QuadVerticesPositions[i];
            s_Data.QuadVertexBufferPtr->Color = color;
            s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
            s_Data.QuadVertexBufferPtr->ObjectID = objectID;

            s_Data.QuadVertexBufferPtr++;
        }
        
        s_Data.QuadIndexCount += 6; // 索引个数增加

        s_Data.Stats.QuadCount++;   // 四边形个数 ++
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const glm::vec4& color, const Ref<Texture2D>& texture)
    {
        DrawQuad({ position.x, position.y, 0.0f }, rotation, { scale.x, scale.y, 1.0f }, color, texture);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, float rotation, const glm::vec3& scale, const glm::vec4& color, const Ref<Texture2D>& texture)
    {
        // 索引个数超过最大索引数
        if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
        {
            NextBatch();    // 开始新一次批渲染
        }

        const int quadVertexCount = 4;  // 顶点个数
        const glm::vec2 texCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };   //纹理坐标
        float texIndex = 0.0f;          // 白色纹理索引

        // Transform 矩阵
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1.0f))
            * glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

        // 遍历所有已存在的纹理
        for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
        {
            // texture 在纹理槽中
            if (*s_Data.TextureSlots[i].get() == *texture.get())
            {
                texIndex = (float)i;                                // 设置当前纹理索引
                break;
            }
        }

        // 当前纹理不在纹理槽中
        if (texIndex == 0.0f)
        {
            //if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
            //{
            //    NextBatch();
            //}

            texIndex = (float)s_Data.TextureSlotIndex;
            s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture; // 添加 texture 到 第 s_Data.TextureSlotIndex 个纹理槽
            s_Data.TextureSlotIndex++;  // 纹理槽索引++
        }

        // 4 个顶点数据
        for (int i = 0; i < quadVertexCount; i++)
        {
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVerticesPositions[i];
            s_Data.QuadVertexBufferPtr->Color = color;
            s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
            s_Data.QuadVertexBufferPtr++;
        }

        s_Data.QuadIndexCount += 6;     // 索引个数增加

        s_Data.Stats.QuadCount++;       // 四边形个数 ++
    }

    Renderer2D::Statistics Renderer2D::GetStats()
    {
        return s_Data.Stats;
    }

    void Renderer2D::ResetStats()
    {
        memset(&s_Data.Stats, 0, sizeof(Statistics));
    }
}