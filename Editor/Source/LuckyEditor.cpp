#include <Lucky.h>

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>

class ExampleLayer : public Lucky::Layer
{
private:
	std::shared_ptr<Lucky::Shader> m_Shader;			// 着色器
	Lucky::ShaderLibrary m_ShaderLibrary;				// 着色器库

	std::shared_ptr<Lucky::VertexArray> m_VertexArray;	// 顶点数组

	std::shared_ptr<Lucky::Shader> m_FlatColorShader;
	std::shared_ptr<Lucky::VertexArray> m_SquareVA;

	std::shared_ptr<Lucky::Texture2D> m_Texture;
	std::shared_ptr<Lucky::Texture2D> m_LuckyLogoTexture;

	Lucky::CameraController m_CameraController;			// 相机控制器

	glm::vec3 m_SquareColor = { 0.2f,0.3f,0.8f };
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.0f / 720.0f)
	{
		m_VertexArray.reset(new Lucky::VertexArray());		// 创建顶点数组对象

		float vertices[] = {
			// ----- 位置 -----  -------- 颜色 --------
			-0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 1.0f, 1.0f,	// 左下
			 0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 1.0f, 1.0f,	// 右下
			 0.0f,  0.5f, 0.0f,	1.0f, 1.0f, 0.0f, 1.0f,	// 上
		};

		std::shared_ptr<Lucky::VertexBuffer> vertexBuffer;								// VBO
		vertexBuffer.reset(new Lucky::VertexBuffer(vertices, sizeof(vertices)));		// 创建顶点缓冲区

		//顶点缓冲区布局
		Lucky::BufferLayout layout = {
			{ Lucky::ShaderDataType::Float3, "a_Position" },	//位置
			{ Lucky::ShaderDataType::Float4, "a_Color" }		//颜色
		};

		vertexBuffer->SetLayout(layout);				// 设置顶点缓冲区布局
		m_VertexArray->AddVertexBuffer(vertexBuffer);	// 添加 VBO 到 VAO

		unsigned int indices[3] = { 0, 1, 2 };			// 顶点索引

		std::shared_ptr<Lucky::IndexBuffer> indexBuffer;										// EBO
		indexBuffer.reset(new Lucky::IndexBuffer(indices, sizeof(indices) / sizeof(uint32_t)));	// 创建索引缓冲
		m_VertexArray->SetIndexBuffer(indexBuffer);												// 设置 EBO 到 VAO

		m_Shader.reset(new Lucky::Shader("Assets/Shaders/Triangle"));	// 创建着色器

		//Square
		m_SquareVA.reset(new Lucky::VertexArray());		// 创建顶点数组对象

		float squareVertices[] = {
			//------位置------  --纹理坐标--
			-0.5f, -0.5f, 0.0f,	0.0f, 0.0f,	// 左下0
			 0.5f, -0.5f, 0.0f,	1.0f, 0.0f,	// 右下1
			-0.5f,  0.5f, 0.0f,	0.0f, 1.0f,	// 左上2
			 0.5f,  0.5f, 0.0f,	1.0f, 1.0f	// 右上3
		};

		std::shared_ptr<Lucky::VertexBuffer> squareVB;										// VBO

		squareVB.reset(new Lucky::VertexBuffer(squareVertices, sizeof(squareVertices)));	// 创建顶点缓冲

		squareVB->SetLayout({
			{ Lucky::ShaderDataType::Float3, "a_Position" },	// 位置
			{ Lucky::ShaderDataType::Float2, "a_TexCoord" },	// 纹理坐标
		});

		m_SquareVA->AddVertexBuffer(squareVB);					// 添加 VBO 到 VAO

		unsigned int squareIndices[] = { 0, 1, 2, 2, 1, 3 };	// 顶点索引

		std::shared_ptr<Lucky::IndexBuffer> squareIB;														// EBO
		squareIB.reset(new Lucky::IndexBuffer(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));	// 创建索引缓冲
		m_SquareVA->SetIndexBuffer(squareIB);																// 设置 EBO 到 VAO

		
		m_FlatColorShader.reset(new Lucky::Shader("Assets/Shaders/FlatColor"));		// 创建着色器

		auto textureShader = m_ShaderLibrary.Load("Assets/Shaders/Texture");		// 加载着色器

		m_Texture.reset(new Lucky::Texture2D("Assets/Textures/Checkerboard.png"));				// 创建纹理
		m_LuckyLogoTexture.reset(new Lucky::Texture2D("Assets/Textures/LuckyLogoBlue.png"));	// 创建纹理

		textureShader->Bind();
		textureShader->UploadUniformInt("u_Texture", 0);
	}

	virtual void OnUpdate(Lucky::DeltaTime dt) override
	{
		LC_TRACE("FPS: {0}", (int)(1000.0f / dt.GetMilliseconds()));

		

		Lucky::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Lucky::RenderCommand::Clear();

		m_CameraController.OnUpdate(dt);	// 更新相机控制器

		Lucky::Renderer::BeginScene(m_CameraController.GetCamera());			// 开始渲染场景

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		m_FlatColorShader->Bind();
		m_FlatColorShader->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;		// 三角形的变换矩阵

				Lucky::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);		// 提交渲染指令
			}
		}

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		Lucky::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_LuckyLogoTexture->Bind();
		Lucky::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));


		Lucky::Renderer::EndScene();						// 结束渲染场景
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");

		ImGui::ColorEdit3("Triangle Color", glm::value_ptr(m_SquareColor));
		
		ImGui::End();
	}

	virtual void OnEvent(Lucky::Event& event) override
	{
		m_CameraController.OnEvent(event);		// 调用相机事件函数
	}
};

class LuckyEditor : public Lucky::Application
{
public:
	LuckyEditor()
	{
		PushLayer(new ExampleLayer());
	}

	~LuckyEditor()
	{

	}
};

Lucky::Application* Lucky::CreateApplication()
{
	return new LuckyEditor();
}