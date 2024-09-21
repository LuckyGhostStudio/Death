#include <Lucky.h>

#include <glm/ext/matrix_transform.hpp>

class ExampleLayer : public Lucky::Layer
{
private:
	std::shared_ptr<Lucky::Shader> m_Shader;			// 着色器
	std::shared_ptr<Lucky::VertexArray> m_VertexArray;	// 顶点数组
	Lucky::Camera m_Camera;								// 相机

	glm::vec3 m_CameraPosition;

	float m_CameraRotation = 0.0f;
	float m_CameraMoveSpeed = 1.0f;
	float m_CameraRotationSpeed = 90.0f;
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
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

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjectionMatrix;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;

				gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
			}
		)";
		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;
			
			void main()
			{
				color = v_Color;
			}
		)";

		m_Shader.reset(new Lucky::Shader(vertexSrc, fragmentSrc));	// 创建着色器
	}

	virtual void OnUpdate(Lucky::DeltaTime dt) override
	{
		LC_TRACE("Delta Time: {0}s ({1}ms)", dt.GetSeconds(), dt.GetMilliseconds());

		if (Lucky::Input::IsKeyPressed(Lucky::Key::Left)) {			// 左键
			m_CameraPosition.x += m_CameraMoveSpeed * dt;
		}
		else if (Lucky::Input::IsKeyPressed(Lucky::Key::Right)) {	// 右键
			m_CameraPosition.x -= m_CameraMoveSpeed * dt;
		}

		if (Lucky::Input::IsKeyPressed(Lucky::Key::Up)) {			// 上键
			m_CameraPosition.y -= m_CameraMoveSpeed * dt;
		}
		else if (Lucky::Input::IsKeyPressed(Lucky::Key::Down)) {	// 下键
			m_CameraPosition.y += m_CameraMoveSpeed * dt;
		}

		if (Lucky::Input::IsKeyPressed(Lucky::Key::A)) {
			m_CameraRotation -= m_CameraRotationSpeed * dt;
		}

		if (Lucky::Input::IsKeyPressed(Lucky::Key::D)) {
			m_CameraRotation += m_CameraRotationSpeed * dt;
		}

		Lucky::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Lucky::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Lucky::Renderer::BeginScene(m_Camera);				// 开始渲染场景
		
		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;	// 三角形的变换矩阵
				Lucky::Renderer::Submit(m_Shader, m_VertexArray, transform);		// 提交渲染指令
			}
		}

		Lucky::Renderer::EndScene();						// 结束渲染场景
	}

	virtual void OnImGuiRender() override
	{

	}

	virtual void OnEvent(Lucky::Event& event) override
	{
		if (event.GetEventType() == Lucky::EventType::KeyPressed) {		// 按键按下事件
			Lucky::KeyPressedEvent& e = (Lucky::KeyPressedEvent&)event;
			LC_TRACE("{0}", (char)e.GetKeyCode());
		}
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