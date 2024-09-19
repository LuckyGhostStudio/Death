#include "lcpch.h"
#include "Application.h"

#include <glad/glad.h>

namespace Lucky
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		LC_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());					// ��������
		m_Window->SetEventCallback(LC_BIND_EVENT_FUNC(Application::OnEvent));	// ���ûص�����

		m_ImGuiLayer = new ImGuiLayer();		// ����ImGui��
		PushOverlay(m_ImGuiLayer);				// ���ImGuiLayer�����ǲ�

		// TODO Temp
		glGenVertexArrays(1, &m_VertexArray);	// ������������
		glBindVertexArray(m_VertexArray);		// ��
		glGenBuffers(1, &m_VertexBuffer);				// ����VBO
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);	// ��VBO��VAO

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,		// ����
			 0.5f, -0.5f, 0.0f,		// ����
			 0.0f,  0.5f, 0.0f		// ��
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);		// �󶨶�������
		glEnableVertexAttribArray(0);													// ����0�Ŷ�������
		//0�Ŷ������� ÿ������3������ float���� ÿ������ռ�ֽ��� λ��������һ�������е�ƫ����
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);	// ���ö������Բ��֣�����λ��

		unsigned int indices[3] = { 0,1,2 };					// ��������
		glGenBuffers(1, &m_IndexBuffer);						// ����EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);	// ��EBO��VAO
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);	// �󶨶�����������

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			out vec3 v_Position;			
			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
			}
		)";
		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
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
		while (m_Running) {
			// TODO Temp
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();

			glBindVertexArray(m_VertexArray);							// ��VAO
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);	// ������������������

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