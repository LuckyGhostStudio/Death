#include "lcpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/ext/matrix_transform.hpp>

namespace Lucky
{
	/// <summary>
	/// 四边形顶点
	/// </summary>
	struct QuadVertex
	{
		glm::vec3 Position;	// 位置
		glm::vec4 Color;	// 颜色
		glm::vec2 TexCoord;	// 纹理坐标
	};

	/// <summary>
	/// 2D 渲染器数据
	/// </summary>
	struct Renderer2DData
	{
		uint32_t MaxQuads = 10000;						// 最大四边形数量
		uint32_t MaxVertices = MaxQuads * 4;			// 最大顶点数
		uint32_t MaxIndices = MaxQuads * 6;				// 最大索引数（4个顶点6个索引）

		std::shared_ptr<VertexArray> QuadVertexArray;	// 四边形顶点数组
		std::shared_ptr<VertexBuffer> QuadVertexBuffer;	// 四边形顶点缓冲区
		std::shared_ptr<Shader> TextureShader;			// 纹理着色器
		std::shared_ptr<Texture2D>	WhiteTexture;		// 白色纹理

		uint32_t QuadIndexCount = 0;					// 四边形索引个数
		QuadVertex* QuadVertexBufferBase = nullptr;		// 顶点数据
		QuadVertex* QuadVertexBufferPtr = nullptr;
	};

	static Renderer2DData s_Data;	// 渲染器数据

	void Renderer2D::Init()
	{
		//s_Data = new Renderer2DData();

		//s_Data->QuadVertexArray = std::make_shared<VertexArray>();		// 创建顶点数组对象

		//float squareVertices[] = {
		//	//------位置------  --纹理坐标--
		//	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,	// 左下 0
		//	 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,	// 右下 1
		//	-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,	// 左上 2
		//	 0.5f,  0.5f, 0.0f, 1.0f, 1.0f	// 右上 3
		//};

		s_Data.QuadVertexArray = std::make_shared<VertexArray>();		// 创建顶点数组对象

		//std::shared_ptr<VertexBuffer> squareVB;										// VBO
		//squareVB.reset(new VertexBuffer(squareVertices, sizeof(squareVertices)));	// 创建顶点缓冲

		s_Data.QuadVertexBuffer = std::make_shared<VertexBuffer>(s_Data.MaxVertices * sizeof(QuadVertex));	// 创建顶点缓冲

		// 设置顶点缓冲区布局
		s_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },	// 位置
			{ ShaderDataType::Float4, "a_Color" },		// 颜色
			{ ShaderDataType::Float2, "a_Texture" },	// 纹理坐标
		});

		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);	// 添加 VBO 到 VAO

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];	// 顶点数据

		//std::shared_ptr<IndexBuffer> squareIB;														// EBO
		//squareIB.reset(new IndexBuffer(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));	// 创建索引缓冲
		//s_Data->QuadVertexArray->SetIndexBuffer(squareIB);											// 设置 EBO 到 VAO

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];	// 顶点索引

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6) {
			quadIndices[i + 0] = offset + 0;	// 左下
			quadIndices[i + 1] = offset + 1;	// 右下
			quadIndices[i + 2] = offset + 2;	// 右上

			quadIndices[i + 3] = offset + 2;	// 右上
			quadIndices[i + 4] = offset + 3;	// 左上
			quadIndices[i + 5] = offset + 0;	// 左下

			offset += 4;	//偏移4个顶点
		}

		std::shared_ptr<IndexBuffer> quadIB = std::make_shared<IndexBuffer>(quadIndices, s_Data.MaxIndices);	// 创建索引缓冲
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);									// 设置 EBO 到 VAO
		delete[] quadIndices;

		s_Data.WhiteTexture = std::make_shared<Texture2D>(1, 1);			// 创建宽高为 1 的纹理
		uint32_t whitTextureData = 0xffffffff;								// 255 白色
		s_Data.WhiteTexture->SetData(&whitTextureData, sizeof(uint32_t));	// 设置纹理数据 size = 1 * 1 * 4 == sizeof(uint32_t)

		s_Data.TextureShader = std::make_shared<Shader>("Assets/Shaders/TextureShader");			// 创建 Texture 着色器

		s_Data.TextureShader->Bind();					// 绑定 Texture 着色器
		s_Data.TextureShader->SetInt("u_Texture", 0);	// 设置 texture 变量
	}

	void Renderer2D::Shutdown()
	{
		
	}

	void Renderer2D::BeginScene(const Camera& camera)
	{
		s_Data.TextureShader->Bind();		// 绑定 Texture 着色器
		s_Data.TextureShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());		// 设置 vp 矩阵

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;	// 初始化顶点数据指针
	}

	void Renderer2D::EndScene()
	{
		uint32_t dataSize = (uint32_t)s_Data.QuadVertexBufferPtr - (uint32_t)s_Data.QuadVertexBufferBase;	// 数据大小（字节）
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);	// 设置顶点缓冲区数据

		Flush();
	}

	void Renderer2D::Flush()
	{
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);	// 绘制
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, rotation, { scale.x, scale.y, 1.0f }, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float rotation, const glm::vec3& scale, const glm::vec4& color)
	{
		//s_Data->TextureShader->SetFloat4("u_Color", color);		// 设置color
		//s_Data->WhiteTexture->Bind();							// 绑定白色纹理（默认纹理）
		
		//左下角顶点数据：锚点在左下角
		s_Data.QuadVertexBufferPtr->Position = position;
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr++;
		//右下角顶点数据
		s_Data.QuadVertexBufferPtr->Position = { position.x + scale.x, position.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr++;
		//右上角顶点数据
		s_Data.QuadVertexBufferPtr->Position = { position.x + scale.x, position.y + scale.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr++;
		//左上角顶点数据
		s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + scale.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr++;
		s_Data.QuadIndexCount += 6;	//索引个数增加

		//glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
		//	* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1.0f))
		//	* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

		//s_Data->TextureShader->SetMat4("u_Transform", transform);		// 设置 transform 矩阵

		//s_Data->QuadVertexArray->Bind();						// 绑定顶点数组

		//RenderCommand::DrawIndexed(s_Data->QuadVertexArray);	// 绘制


	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const glm::vec4& color, const std::shared_ptr<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, rotation, { scale.x, scale.y, 1.0f }, color, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float rotation, const glm::vec3& scale, const glm::vec4& color, const std::shared_ptr<Texture2D>& texture)
	{
		s_Data.TextureShader->SetFloat4("u_Color", color);		// 设置颜色（默认颜色）
		texture->Bind();										// 绑定纹理

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1.0f))
			* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

		s_Data.TextureShader->SetMat4("u_Transform", transform);		// 设置 transform 矩阵

		s_Data.QuadVertexArray->Bind();						// 绑定顶点数组
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);	// 绘制
	}
}