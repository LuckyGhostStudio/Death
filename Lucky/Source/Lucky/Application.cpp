#include "lcpch.h"
#include "Application.h"

#include "Lucky/Renderer/Renderer.h"

#include <glad/glad.h>

namespace Lucky
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		LC_CORE_ASSERT(!s_Instance, "Application already exists!");

		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());					// ��������
		m_Window->SetEventCallback(LC_BIND_EVENT_FUNC(Application::OnEvent));	// ���ûص�����

		m_ImGuiLayer = new ImGuiLayer();		// ����ImGui��
		PushOverlay(m_ImGuiLayer);				// ���ImGuiLayer�����ǲ�

		m_VertexArray.reset(new VertexArray());		// ���������������

		float vertices[] = {
			// ----- λ�� -----  -------- ��ɫ --------
			-0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 1.0f, 1.0f,	// ����
			 0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 1.0f, 1.0f,	// ����
			 0.0f,  0.5f, 0.0f,	1.0f, 1.0f, 0.0f, 1.0f,	// ��
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;								// VBO
		vertexBuffer.reset(new VertexBuffer(vertices, sizeof(vertices)));		// �������㻺����

		//���㻺��������
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },	//λ��
			{ ShaderDataType::Float4, "a_Color" }		//��ɫ
		};

		vertexBuffer->SetLayout(layout);				// ���ö��㻺��������
		m_VertexArray->AddVertexBuffer(vertexBuffer);	// ��� VBO �� VAO

		unsigned int indices[3] = { 0, 1, 2 };			// ��������

		std::shared_ptr<IndexBuffer> indexBuffer;												// EBO
		indexBuffer.reset(new IndexBuffer(indices, sizeof(indices) / sizeof(uint32_t)));		// ������������
		m_VertexArray->SetIndexBuffer(indexBuffer);												// ���� EBO �� VAO

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjectionMatrix;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;

				gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
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

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));	// ������ɫ��
	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);	// �¼�������
		dispatcher.Dispatch<WindowCloseEvent>(LC_BIND_EVENT_FUNC(Application::OnWindowClose));	// ���� ���ڹر��¼�

		// ��������±�����ջ
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);		// ���ȡ�������¼�
			if (e.GetHandled()) break;	// �¼��Ѵ���
		}
	}

	void Application::Run()
	{
		float angle = 0.0f;

		while (m_Running) {
			// TODO Temp
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RenderCommand::Clear();

			m_Camera.SetPosition({ 0.5f, 0.5f, 0.0f });
			m_Camera.SetRotation(angle += 0.2f);

			Renderer::BeginScene(m_Camera);				// ��ʼ��Ⱦ����
			Renderer::Submit(m_Shader, m_VertexArray);	// �ύ��Ⱦ����
			Renderer::EndScene();						// ������Ⱦ����

			// ���²�ջ�����в�
			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}

			// ImGui��Ⱦ
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack) {
				layer->OnImGuiRender();		// ��Ⱦÿ��Layer��ImGui
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();			// ���´���
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;	// ��������
		return true;
	}
}