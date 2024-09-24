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
		/// �����ӿڴ�С
		/// </summary>
		/// <param name="x">���� x</param>
		/// <param name="y">���� y</param>
		/// <param name="width">��</param>
		/// <param name="height">��</param>
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

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