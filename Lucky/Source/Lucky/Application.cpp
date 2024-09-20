#include "lcpch.h"
#include "Application.h"

#include <glad/glad.h>

namespace Lucky
{
	Application* Application::s_Instance = nullptr;

	/// <summary>
	/// ShaderDataType转换为OpenGL基本类型
	/// </summary>
	/// <param name="">ShaderDataType</param>
	/// <returns>OpenGL基本类型</returns>
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

		m_Window = std::unique_ptr<Window>(Window::Create());					// 创建窗口
		m_Window->SetEventCallback(LC_BIND_EVENT_FUNC(Application::OnEvent));	// 设置回调函数

		m_ImGuiLayer = new ImGuiLayer();		// 创建ImGui层
		PushOverlay(m_ImGuiLayer);				// 添加ImGuiLayer到覆盖层

		// TODO Temp
		glGenVertexArrays(1, &m_VertexArray);	// 创建顶点数组
		glBindVertexArray(m_VertexArray);		// 绑定

		float vertices[] = {
			// ----- 位置 -----  -------- 颜色 --------
			-0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 1.0f, 1.0f,	// 左下
			 0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 1.0f, 1.0f,	// 右下
			 0.0f,  0.5f, 0.0f,	1.0f, 1.0f, 0.0f, 1.0f,	// 上
		};

		m_VertexBuffer.reset(new VertexBuffer(vertices, sizeof(vertices)));				// 创建顶点缓冲区

		{
			// 顶点缓冲区布局（出作用域销毁）
			BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position" },	// 位置
				{ ShaderDataType::Float4, "a_Color" }		// 颜色
			};
			m_VertexBuffer->SetLayout(layout);	// 设置顶点缓冲区布局
		}

		const auto& layout = m_VertexBuffer->GetLayout();		// 顶点缓冲区布局

		uint32_t index = 0;
		for (const auto& element : layout) {
			glEnableVertexAttribArray(index);					// 启用顶点属性
			glVertexAttribPointer(index,						// 顶点属性位置编号
				element.GetComponentCount(),					// 顶点属性数据个数
				ShaderDataTypeToOpenGLBaseType(element.Type), 	// 数据类型
				element.Normalized ? GL_TRUE : GL_FALSE, 		// 是否标准化
				layout.GetStride(), 							// 顶点大小（字节）
				(const void*)element.Offset);					// 顶点属性偏移量（字节）
			index++;
		}

		unsigned int indices[3] = { 0, 1, 2 };					// 顶点索引
		m_IndexBuffer.reset(new IndexBuffer(indices, sizeof(indices) / sizeof(uint32_t)));	// 创建索引缓冲区

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

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));	// 创建着色器
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
		EventDispatcher dispatcher(e);	// 事件调度器
		dispatcher.Dispatch<WindowCloseEvent>(LC_BIND_EVENT_FUNC(Application::OnWindowClose));	// 调度 窗口关闭事件

		// 从最顶层向下遍历层栈
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);		// 层获取并处理事件
			if (e.GetHandled()) break;	// 事件已处理
		}
	}

	void Application::Run()
	{
		while (m_Running) {
			// TODO Temp
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();

			glBindVertexArray(m_VertexArray);							// 绑定VAO
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);	// 根据索引绘制三角形

			// 更新层栈中所有层
			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}

			// ImGui渲染
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack) {
				layer->OnImGuiRender();		// 渲染每个Layer的ImGui
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();			// 更新窗口
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;	// 结束运行
		return true;
	}
}