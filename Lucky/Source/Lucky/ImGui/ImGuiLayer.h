#pragma once

#include "Lucky/Layer.h"

#include "Lucky/Events/ApplicationEvent.h"
#include "Lucky/Events/KeyEvent.h"
#include "Lucky/Events/MouseEvent.h"

namespace Lucky
{
	/// <summary>
	/// ImGui��
	/// </summary>
	class ImGuiLayer : public Layer
	{
	private:
		float m_Time = 0.0f;	//��ǰ֡ʱ��
	public:
		ImGuiLayer();
		~ImGuiLayer();

		/// <summary>
		/// ���ò���ӵ���ջʱ����
		/// </summary>
		void OnAttach();

		/// <summary>
		/// ���ò�Ӳ�ջ�Ƴ�ʱ����
		/// </summary>
		void OnDetach();

		/// <summary>
		/// �ò����ʱ���ã�ÿ֡����
		/// </summary>
		void OnUpdate();

		/// <summary>
		/// �¼������������¼�
		/// </summary>
		/// <param name="event">�¼�</param>
		void OnEvent(Event& event);
	private:
		/// <summary>
		/// ��갴ť����
		/// </summary>
		/// <param name="event">��갴ť�����¼�</param>
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& event);

		/// <summary>
		/// ��갴ţ̌��
		/// </summary>
		/// <param name="event">��갴ţ̌���¼�</param>
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event);

		/// <summary>
		/// ����ƶ�
		/// </summary>
		/// <param name="event">����ƶ��¼�</param>
		bool OnMouseMovedEvent(MouseMovedEvent& event);

		/// <summary>
		/// �����ֹ���
		/// </summary>
		/// <param name="event">�����ֹ����¼�</param>
		bool OnMouseScrolledEvent(MouseScrolledEvent& event);

		/// <summary>
		/// ���̰�������
		/// </summary>
		/// <param name="event">���̰��������¼�</param>
		bool OnKeyPressedEvent(KeyPressedEvent& event);

		/// <summary>
		/// ���̰���̧��
		/// </summary>
		/// <param name="event">���̰���̧���¼�</param>
		bool OnKeyReleasedEvent(KeyReleasedEvent& event);

		/// <summary>
		/// �������밴���ַ�
		/// </summary>
		/// <param name="event">���������ַ��¼�</param>
		/// <returns></returns>
		bool OnKeyTypedEvent(KeyTypedEvent& event);

		/// <summary>
		/// ���ڴ�С�ı�
		/// </summary>
		/// <param name="event">���ڴ�С�ı��¼�</param>
		bool OnWindowResizeEvent(WindowResizeEvent& event);
	};
}