#pragma once

#include <string>

#include <glm/glm.hpp>

namespace Lucky
{
	/// <summary>
	/// ��ɫ��
	/// </summary>
	class Shader
	{
	private:
		uint32_t m_RendererID;	// ��ɫ�� ID
	public:
		/// <summary>
		/// ��ɫ��
		/// </summary>
		/// <param name="vertexSrc">������ɫ��Դ����</param>
		/// <param name="fragmentSrc">Ƭ����ɫ��Դ����</param>
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		
		~Shader();

		/// <summary>
		/// �󶨣�ʹ����ɫ��
		/// </summary>
		void Bind() const;

		/// <summary>
		/// ����󶨣�����ʱʹ��
		/// </summary>
		void UnBind() const;

		// ---- ���з������ϴ� Uniform ������ Shader ---- |�������� Shader �еı�����������ֵ��

		void UploadUniformInt(const std::string& name, int value);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	};
}