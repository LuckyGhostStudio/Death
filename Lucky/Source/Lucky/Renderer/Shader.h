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

		/// <summary>
		/// ���ļ�
		/// </summary>
		/// <param name="filepath">�ļ�·��</param>
		/// <returns>�ļ�����</returns>
		std::string ReadFile(const std::string& filepath);

		/// <summary>
		/// ������ɫ��
		/// </summary>
		/// <param name="shaderSources">��ɫ������ - ��ɫ��Դ�� map</param>
		void Compile(std::unordered_map<unsigned int, std::string>& shaderSources);
	public:
		/// <summary>
		/// ��ɫ��
		/// </summary>
		/// <param name="vertexShaderPath">������ɫ���ļ�·��</param>
		/// <param name="fragmentShaderPath">Ƭ����ɫ���ļ�·��</param>
		Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
		
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