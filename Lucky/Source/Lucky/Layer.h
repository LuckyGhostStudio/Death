#pragma once

#include "Lucky/Core/Core.h"
#include "Lucky/Core/DeltaTime.h"
#include "Lucky/Events/Event.h"

namespace Lucky
{
	class Layer
	{
	protected:
		std::string m_DebugName;	// Layer �� Debug ģʽ�е�����
	public:
		Layer(const std::string& name = "Layer");

		virtual ~Layer();

		/// <summary>
		/// ���ò���ӵ���ջʱ����
		/// </summary>
		virtual void OnAttach() {}

		/// <summary>
		/// ���ò�Ӳ�ջ�Ƴ�ʱ����
		/// </summary>
		virtual void OnDetach() {}

		/// <summary>
		/// �ò����ʱ���ã�ÿ֡����
		/// </summary>
		/// <param name="dt">֡���</param>
		virtual void OnUpdate(DeltaTime dt) {}

		/// <summary>
		/// ��ȾImGui
		/// </summary>
		virtual void OnImGuiRender() {}

		/// <summary>
		/// �¼������������¼�
		/// </summary>
		/// <param name="event">�¼�</param>
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
	};
}