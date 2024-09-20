#pragma once

#include "Lucky/Core.h"
#include "Lucky/LayerStack.h"
#include "Lucky/Events/Event.h"
#include "Lucky/Events/ApplicationEvent.h"
#include "Lucky/Window.h"

#include "Lucky/ImGui/ImGuiLayer.h"

#include "Lucky/Renderer/Buffer.h"
#include "Lucky/Renderer/Shader.h"

namespace Lucky
{
	class Application
	{
	private:
		static Application* s_Instance;		// ��̬ʵ��

		std::unique_ptr<Window> m_Window;	// ����ָ��
		ImGuiLayer* m_ImGuiLayer;			// ImGui��ָ��

		bool m_Running = true;				// �Ƿ���������
		LayerStack m_LayerStack;			// ��ջ

		unsigned int m_VertexArray;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;	// ���㻺����
		std::unique_ptr<IndexBuffer> m_IndexBuffer;		// ����������
		std::unique_ptr<Shader> m_Shader;				// ��ɫ��
	public:
		Application();
		virtual ~Application();

		/// <summary>
		/// �����ͨ�㵽��ջ
		/// </summary>
		/// <param name="layer">��ͨ��</param>
		void PushLayer(Layer* layer);

		/// <summary>
		/// ��Ӹ��ǲ㵽��ջ
		/// </summary>
		/// <param name="layer">���ǲ�</param>
		void PushOverlay(Layer* layer);

		/// <summary>
		/// �¼��ص�����
		/// </summary>
		/// <param name="e">�¼�</param>
		void OnEvent(Event& e);

		void Run();

		/// <summary>
		/// ����Application��ʵ��
		/// </summary>
		/// <returns>ʵ��</returns>
		inline static Application& GetInstance() { return *s_Instance; }

		/// <summary>
		/// ����Application�Ĵ���
		/// </summary>
		/// <returns></returns>
		inline Window& GetWindow() { return *m_Window; }
	private:
		/// <summary>
		/// ���ڹرջص�����
		/// </summary>
		/// <param name="e">���ڹر��¼�</param>
		/// <returns>�Ƿ��ѹر�</returns>
		bool OnWindowClose(WindowCloseEvent& e);
	};

	/// <summary>
	/// ���� App���� LuckyEditor �ж���
	/// </summary>
	/// <returns></returns>
	Application* CreateApplication();
}