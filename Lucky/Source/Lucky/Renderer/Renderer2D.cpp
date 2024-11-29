#include "lcpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "UniformBuffer.h"
#include "RenderCommand.h"

#include <glm/ext/matrix_transform.hpp>

namespace Lucky
{
    /// <summary>
    /// �ı��ζ���
    /// </summary>
    struct QuadVertex
    {
        glm::vec3 Position; // λ��
        glm::vec4 Color;    // ��ɫ
        glm::vec2 TexCoord; // ��������
        float TexIndex;     // ��������

        int ObjectID;       // ���� ID
    };

    /// <summary>
    /// ֱ�߶���
    /// </summary>
    struct LineVertex
    {
        glm::vec3 Position; // λ��
        glm::vec4 Color;    // ��ɫ

        int ObjectID;
    };

    /// <summary>
    /// 2D ��Ⱦ������
    /// </summary>
    struct Renderer2DData
    {
        static const uint32_t MaxQuads = 10000;             // ����ı�������
        static const uint32_t MaxVertices = MaxQuads * 4;   // ��󶥵���
        static const uint32_t MaxIndices = MaxQuads * 6;    // �����������4 ������ 6 ��������
        static const uint32_t MaxTextureSlots = 32;         // ����������

        Ref<VertexArray> QuadVertexArray;   // �ı��ζ�������
        Ref<VertexBuffer> QuadVertexBuffer; // �ı��ζ��㻺����
        Ref<Shader> TextureShader;          // ������ɫ��
        Ref<Texture2D> WhiteTexture;        // ��ɫ����

        Ref<VertexArray> LineVertexArray;   // ֱ�߶�������
        Ref<VertexBuffer> LineVertexBuffer; // ֱ�߶��㻺����
        Ref<Shader> LineShader;             // ֱ����ɫ��

        uint32_t QuadIndexCount = 0;                // �ı�����������
        QuadVertex* QuadVertexBufferBase = nullptr; // ��������
        QuadVertex* QuadVertexBufferPtr = nullptr;  // ��������ָ��

        uint32_t LineVertexCount = 0;               // ֱ�߶������
        LineVertex* LineVertexBufferBase = nullptr; // ֱ�߶�������    
        LineVertex* LineVertexBufferPtr = nullptr;  // ֱ�߶�������ָ��
        float LineWidth = 2.0f;                     // �߿�

        std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;   // ������б� �洢����
        uint32_t TextureSlotIndex = 1;                              // ��������� 0 = White

        glm::vec4 QuadVerticesPositions[4];     // ����λ��
        Renderer2D::Statistics Stats;           // ͳ������

        /// <summary>
        /// �������
        /// </summary>
        struct CameraData
        {
            glm::mat4 ViewProjectionMatrix;     // VP ����
        };

        CameraData CameraBuffer;
        Ref<UniformBuffer> CameraUniformBuffer; // ��� Uniform ������
    };

    static Renderer2DData s_Data;   // ��Ⱦ������

    void Renderer2D::Init()
    {
        s_Data.QuadVertexArray = VertexArray::Create(); // ���������������

        s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));    // �������㻺��

        // ���ö��㻺��������
        s_Data.QuadVertexBuffer->SetLayout(
        {
            { ShaderDataType::Float3, "a_Position" },   // λ��
            { ShaderDataType::Float4, "a_Color" },      // ��ɫ
            { ShaderDataType::Float2, "a_Texture" },    // ��������
            { ShaderDataType::Float, "a_TexIndex" },    // ��������
            { ShaderDataType::Int, "a_ObjectID" },      // ���� ID
        });

        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);   // ��� IndexBuffer

        s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];   // ��������

        uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];            // ��������

        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
        {
            quadIndices[i + 0] = offset + 0;    // ����
            quadIndices[i + 1] = offset + 1;    // ����
            quadIndices[i + 2] = offset + 2;    // ����

            quadIndices[i + 3] = offset + 2;    // ����
            quadIndices[i + 4] = offset + 3;    // ����
            quadIndices[i + 5] = offset + 0;    // ����

            offset += 4;    // ƫ�� 4 ������
        }

        Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);  // ������������
        s_Data.QuadVertexArray->SetIndexBuffer(quadIB);                                 // ���� IndexBuffer
        delete[] quadIndices;

        // Lines
        s_Data.LineVertexArray = VertexArray::Create();
        s_Data.LineVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(LineVertex));

        s_Data.LineVertexBuffer->SetLayout(
        {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color"    },
            { ShaderDataType::Int,    "a_ObjectID" }
        });
        s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);
        s_Data.LineVertexBufferBase = new LineVertex[s_Data.MaxVertices];

        s_Data.WhiteTexture = Texture2D::Create(1, 1);                      // �������Ϊ 1 ������
        uint32_t whitTextureData = 0xffffffff;                              // 255 ��ɫ
        s_Data.WhiteTexture->SetData(&whitTextureData, sizeof(uint32_t));   // ������������ size = 1 * 1 * 4 == sizeof(uint32_t)

        // ��������� 0 - 31
        int samplers[s_Data.MaxTextureSlots];
        for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
        {
            samplers[i] = i;
        }

        s_Data.TextureShader = Shader::Create("Assets/Shaders/TextureShader");  // ���� Texture ��ɫ��
        s_Data.LineShader = Shader::Create("Assets/Shaders/LineShader");        // ���� Line ��ɫ��

        s_Data.TextureSlots[0] = s_Data.WhiteTexture;   // 0 �������Ϊ��ɫ����Ĭ�ϣ�

        s_Data.QuadVerticesPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.QuadVerticesPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.QuadVerticesPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
        s_Data.QuadVerticesPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

        s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);  // ������� Uniform ������
    }

    void Renderer2D::Shutdown()
    {
        delete[] s_Data.QuadVertexBufferBase;
    }

    void Renderer2D::BeginScene(const EditorCamera& camera)
    {
        s_Data.CameraBuffer.ViewProjectionMatrix = camera.GetViewProjectionMatrix();                    // ���� VP ����
        s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));  // ���� Uniform ����������

        StartBatch();   // ��ʼ����Ⱦ
    }

    void Renderer2D::BeginScene(const Camera& camera, const Transform& transform)
    {
        glm::mat4 viewProjectMatrix = camera.GetProjectionMatrix() * glm::inverse(transform.GetTransform());    // VP = P * V

        s_Data.CameraBuffer.ViewProjectionMatrix = viewProjectMatrix;                                   // ���� VP ����
        s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));  // ���� Uniform ����������

        StartBatch();   // ��ʼ����Ⱦ
    }

    void Renderer2D::EndScene()
    {
        Flush();
    }

    void Renderer2D::StartBatch()
    {
        // Quad
        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;   // ��ʼ����������ָ��

        // Line
        s_Data.LineVertexCount = 0;
        s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;

        s_Data.TextureSlotIndex = 1;    // ����������� 1 ��ʼ 0 Ϊ��ɫ����
    }

    void Renderer2D::Flush()
    {
        // Quad
        if (s_Data.QuadIndexCount)
        {
            uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);   // ���ݴ�С���ֽڣ�
            s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);    // ���ö��㻺��������

            // ����������
            for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
            {
                s_Data.TextureSlots[i]->Bind(i);    // �� i �������
            }

            s_Data.TextureShader->Bind();   // �� Texture ��ɫ��

            RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);  // ����

            s_Data.Stats.DrawCalls++;       // ���Ƶ��ô��� ++
        }

        // Line
        if (s_Data.LineVertexCount)
        {
            uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase);
            s_Data.LineVertexBuffer->SetData(s_Data.LineVertexBufferBase, dataSize);

            s_Data.LineShader->Bind();

            RenderCommand::SetLineWidth(s_Data.LineWidth);
            RenderCommand::DrawLines(s_Data.LineVertexArray, s_Data.LineVertexCount);

            s_Data.Stats.DrawCalls++;
        }
    }

    void Renderer2D::NextBatch()
    {
        Flush();

        StartBatch();
    }

    void Renderer2D::DrawQuad(const Transform& transform, const glm::vec4& color, int objectID)
    {
        // ���������������������
        if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
        {
            NextBatch();    // ��ʼ��һ������Ⱦ
        }

        constexpr int quadVertexCount = 4;  // �������
        constexpr glm::vec2 texCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };   // ��������
        const float texIndex = 0.0f;        // ��ɫ��������

        // Transform ����
        const glm::mat4& transformMat = transform.GetTransform();

        // 4 ����������
        for (int i = 0; i < quadVertexCount; i++)
        {
            s_Data.QuadVertexBufferPtr->Position = transformMat * s_Data.QuadVerticesPositions[i];
            s_Data.QuadVertexBufferPtr->Color = color;
            s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
            s_Data.QuadVertexBufferPtr->ObjectID = objectID;

            s_Data.QuadVertexBufferPtr++;
        }
        
        s_Data.QuadIndexCount += 6; // ������������

        s_Data.Stats.QuadCount++;   // �ı��θ��� ++
    }

    void Renderer2D::DrawQuad(const Transform& transform, const glm::vec4& color, Ref<Texture2D>& texture, int objectID)
    {
        // ���������������������
        if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
        {
            NextBatch();    // ��ʼ��һ������Ⱦ
        }

        constexpr int quadVertexCount = 4;  // �������
        constexpr glm::vec2 texCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };   // ��������
        float texIndex = 0.0f;        // ��ɫ��������

        // Transform ����
        const glm::mat4& transformMat = transform.GetTransform();

        // Texture ������ TODO Temp
        if (!texture)
        {
            texture = s_Data.WhiteTexture;  // Ĭ�ϰ�ɫ����
        }

        // ���������Ѵ��ڵ�����
        for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
        {
            // texture ���������
            if (*s_Data.TextureSlots[i].get() == *texture.get())
            {
                texIndex = (float)i;    // ���õ�ǰ��������
                break;
            }
        }

        // ��ǰ�������������
        if (texIndex == 0.0f)
        {
            //if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
            //{
            //    NextBatch();
            //}

            texIndex = (float)s_Data.TextureSlotIndex;
            s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture; // ��� texture �� �� s_Data.TextureSlotIndex �������
            s_Data.TextureSlotIndex++;  // ���������++
        }

        // 4 ����������
        for (int i = 0; i < quadVertexCount; i++)
        {
            s_Data.QuadVertexBufferPtr->Position = transformMat * s_Data.QuadVerticesPositions[i];
            s_Data.QuadVertexBufferPtr->Color = color;
            s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
            s_Data.QuadVertexBufferPtr->ObjectID = objectID;

            s_Data.QuadVertexBufferPtr++;
        }

        s_Data.QuadIndexCount += 6;     // ������������

        s_Data.Stats.QuadCount++;       // �ı��θ��� ++
    }

    void Renderer2D::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int objectID)
    {
        // ���
        s_Data.LineVertexBufferPtr->Position = p0;
        s_Data.LineVertexBufferPtr->Color = color;
        s_Data.LineVertexBufferPtr->ObjectID = objectID;
        s_Data.LineVertexBufferPtr++;

        // �յ�
        s_Data.LineVertexBufferPtr->Position = p1;
        s_Data.LineVertexBufferPtr->Color = color;
        s_Data.LineVertexBufferPtr->ObjectID = objectID;
        s_Data.LineVertexBufferPtr++;

        s_Data.LineVertexCount += 2;    // Line ������� +2
    }

    float Renderer2D::GetLineWidth()
    {
        return s_Data.LineWidth;
    }

    void Renderer2D::SetLineWidth(float width)
    {
        s_Data.LineWidth = width;
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