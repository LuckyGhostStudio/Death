#include <Lucky.h>

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>

class ExampleLayer : public Lucky::Layer
{
private:
	std::shared_ptr<Lucky::Shader> m_Shader;			// ��ɫ��
	std::shared_ptr<Lucky::VertexArray> m_VertexArray;	// ��������

	std::shared_ptr<Lucky::Shader> m_FlatColorShader, m_TextureShader;
	std::shared_ptr<Lucky::VertexArray> m_SquareVA;
	std::shared_ptr<Lucky::Texture2D> m_Texture;

	Lucky::Camera m_Camera;								// ���

	glm::vec3 m_CameraPosition;

	float m_CameraRotation = 0.0f;
	float m_CameraMoveSpeed = 1.0f;
	float m_CameraRotationSpeed = 90.0f;

	glm::vec3 m_SquareColor = { 0.2f,0.3f,0.8f };
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		m_VertexArray.reset(new Lucky::VertexArray());		// ���������������

		float vertices[] = {
			// ----- λ�� -----  -------- ��ɫ --------
			-0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 1.0f, 1.0f,	// ����
			 0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 1.0f, 1.0f,	// ����
			 0.0f,  0.5f, 0.0f,	1.0f, 1.0f, 0.0f, 1.0f,	// ��
		};

		std::shared_ptr<Lucky::VertexBuffer> vertexBuffer;								// VBO
		vertexBuffer.reset(new Lucky::VertexBuffer(vertices, sizeof(vertices)));		// �������㻺����

		//���㻺��������
		Lucky::BufferLayout layout = {
			{ Lucky::ShaderDataType::Float3, "a_Position" },	//λ��
			{ Lucky::ShaderDataType::Float4, "a_Color" }		//��ɫ
		};

		vertexBuffer->SetLayout(layout);				// ���ö��㻺��������
		m_VertexArray->AddVertexBuffer(vertexBuffer);	// ��� VBO �� VAO

		unsigned int indices[3] = { 0, 1, 2 };			// ��������

		std::shared_ptr<Lucky::IndexBuffer> indexBuffer;										// EBO
		indexBuffer.reset(new Lucky::IndexBuffer(indices, sizeof(indices) / sizeof(uint32_t)));	// ������������
		m_VertexArray->SetIndexBuffer(indexBuffer);												// ���� EBO �� VAO

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjectionMatrix;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;

				gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
			}
		)";
		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;
			
			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		m_Shader.reset(new Lucky::Shader(vertexSrc, fragmentSrc));	// ������ɫ��

		//Square
		m_SquareVA.reset(new Lucky::VertexArray());		// ���������������

		float squareVertices[] = {
			//------λ��------  --��������--
			-0.5f, -0.5f, 0.0f,	0.0f, 0.0f,	// ����0
			 0.5f, -0.5f, 0.0f,	1.0f, 0.0f,	// ����1
			-0.5f,  0.5f, 0.0f,	0.0f, 1.0f,	// ����2
			 0.5f,  0.5f, 0.0f,	1.0f, 1.0f	// ����3
		};

		std::shared_ptr<Lucky::VertexBuffer> squareVB;										// VBO

		squareVB.reset(new Lucky::VertexBuffer(squareVertices, sizeof(squareVertices)));	// �������㻺��

		squareVB->SetLayout({
			{ Lucky::ShaderDataType::Float3, "a_Position" },	// λ��
			{ Lucky::ShaderDataType::Float2, "a_TexCoord" },	// ��������
		});

		m_SquareVA->AddVertexBuffer(squareVB);					// ��� VBO �� VAO

		unsigned int squareIndices[] = { 0, 1, 2, 2, 1, 3 };	// ��������

		std::shared_ptr<Lucky::IndexBuffer> squareIB;														// EBO
		squareIB.reset(new Lucky::IndexBuffer(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));	// ������������
		m_SquareVA->SetIndexBuffer(squareIB);																// ���� EBO �� VAO

		std::string squareVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjectionMatrix;
			uniform mat4 u_Transform;					

			void main()
			{
				gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string squareFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			uniform vec3 u_Color;
			
			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		m_FlatColorShader.reset(new Lucky::Shader(squareVertexSrc, squareFragmentSrc));	// ������ɫ��

		std::string textureVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjectionMatrix;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;					

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string textureFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec2 v_TexCoord;

			uniform sampler2D u_Texture;
			
			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";

		m_TextureShader.reset(new Lucky::Shader(textureVertexSrc, textureFragmentSrc));	// ������ɫ��

		m_Texture.reset(new Lucky::Texture2D("Assets/Textures/Checkerboard.png"));		// ��������

		m_TextureShader->Bind();
		m_TextureShader->UploadUniformInt("u_Texture", 0);
	}

	virtual void OnUpdate(Lucky::DeltaTime dt) override
	{
		LC_TRACE("Delta Time: {0}s ({1}ms)", dt.GetSeconds(), dt.GetMilliseconds());

		if (Lucky::Input::IsKeyPressed(Lucky::Key::Left)) {			// ���
			m_CameraPosition.x += m_CameraMoveSpeed * dt;
		}
		else if (Lucky::Input::IsKeyPressed(Lucky::Key::Right)) {	// �Ҽ�
			m_CameraPosition.x -= m_CameraMoveSpeed * dt;
		}

		if (Lucky::Input::IsKeyPressed(Lucky::Key::Up)) {			// �ϼ�
			m_CameraPosition.y -= m_CameraMoveSpeed * dt;
		}
		else if (Lucky::Input::IsKeyPressed(Lucky::Key::Down)) {	// �¼�
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

		Lucky::Renderer::BeginScene(m_Camera);				// ��ʼ��Ⱦ����
		
		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		m_FlatColorShader->Bind();
		m_FlatColorShader->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;		//�����εı任����

				Lucky::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);	//�ύ��Ⱦָ��
			}
		}

		m_Texture->Bind();
		Lucky::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));


		Lucky::Renderer::EndScene();						// ������Ⱦ����
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");

		ImGui::ColorEdit3("Triangle Color", glm::value_ptr(m_SquareColor));
		
		ImGui::End();
	}

	virtual void OnEvent(Lucky::Event& event) override
	{
		if (event.GetEventType() == Lucky::EventType::KeyPressed) {		// ���������¼�
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