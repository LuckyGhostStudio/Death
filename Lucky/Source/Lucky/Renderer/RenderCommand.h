#pragma once

#include "Lucky/Renderer/VertexArray.h"

#include <glm/glm.hpp>

namespace Lucky
{
	/// <summary>
	/// ��Ⱦ�����Ⱦ�ӿ������������Ⱦ����
	/// </summary>
	class RenderCommand
	{
	public:
		/// <summary>
		/// ��ʼ����Ⱦ��
		/// </summary>
		static void Init();

		/// <summary>
		/// ����������ɫ
		/// </summary>
		/// <param name="color">������ɫ</param>
		static void SetClearColor(const glm::vec4& color);

		/// <summary>
		/// ����
		/// </summary>
		static void Clear();

		/// <summary>
		/// ��������������
		/// </summary>
		/// <param name="vertexArray">�����ƵĶ�������</param>
		static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray);
	};
}