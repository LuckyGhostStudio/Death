#include "lcpch.h"
#include "Application.h"

#include <glad/glad.h>

namespace Lucky
{
	Application* Application::s_Instance = nullptr;

	/// <summary>
	/// ShaderDataTypeת��ΪOpenGL��������
	/// </summary>
	/// <param name="">ShaderDataType</param>
	/// <returns>OpenGL��������</returns>
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:		return GL_FLOAT;
			case ShaderDataType::Float2:	return GL_FLOAT;
			case ShaderDataType::Float3:	return GL_FLOAT;
			case ShaderDataType::Float4:	return GL_FLOAT;
			case ShaderDataType::Mat3:		return GL_FLOAT;
			case ShaderDataType::Mat4:		return GL_FLOAT;
			case ShaderDataType::Int:		return GL_INT;
			case ShaderDataType::Int2:		return GL_INT;
			case ShaderDataType::Int3:		return GL_INT;
			case ShaderDataType::Int4:		return GL_INT;
			case ShaderDataType::Bool:		return GL_BOOL;
		}

		LC_CORE_ASSERT(false, "Unknown ShaderDataType!");

		return 0;
	}

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

		float vertices[] = {
			// ----- λ�� -----  -------- ��ɫ --------
			-0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 1.0f, 1.0f,	// ����
			 0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 1.0f, 1.0f,	// ����
			 0.0f,  0.5f, 0.0f,	1.0f, 1.0f, 0.0f, 1.0f,	// ��
		};

		m_VertexBuffer.reset(new VertexBuffer(vertices, sizeof(vertices)));				// �������㻺����

		{
			// ���㻺�������֣������������٣�
			BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position" },	// λ��
				{ ShaderDataType::Float4, "a_Color" }		// ��ɫ
			};
			m_VertexBuffer->SetLayout(layout);	// ���ö��㻺��������
		}

		const auto& layout = m_VertexBuffer->GetLayout();		// ���㻺��������

		uint32_t index = 0;
		for (const auto& element : layout) {
			glEnableVertexAttribArray(index);					// ���ö�������
			glVertexAttribPointer(index,						// ��������λ�ñ��
				element.GetComponentCount(),					// �����������ݸ���
				ShaderDataTypeToOpenGLBaseType(element.Type), 	// ��������
				element.Normalized ? GL_TRUE : GL_FALSE, 		// �Ƿ��׼��
				layout.GetStride(), 							// �����С���ֽڣ�
				(const void*)element.Offset);					// ��������ƫ�������ֽڣ�
			index++;
		}

		unsigned int indices[3] = { 0, 1, 2 };					// ��������
		m_IndexBuffer.reset(new IndexBuffer(indices, sizeof(indices) / sizeof(uint32_t)));	// ��������������

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
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
		while (m_Running) {
			// TODO Temp
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();

			glBindVertexArray(m_VertexArray);							// ��VAO
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);	// ������������������

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