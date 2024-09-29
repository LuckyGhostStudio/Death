#pragma once

#include <string>

#include <glm/glm.hpp>

namespace Lucky
{
	/// <summary>
	/// 着色器
	/// </summary>
	class Shader
	{
	private:
		uint32_t m_RendererID;	// 着色器 ID
		std::string m_Name;		// 着色器名字

		/// <summary>
		/// 读文件
		/// </summary>
		/// <param name="filepath">文件路径</param>
		/// <returns>文件内容</returns>
		std::string ReadFile(const std::string& filepath);

		/// <summary>
		/// 编译着色器
		/// </summary>
		/// <param name="shaderSources">着色器类型 - 着色器源码 map</param>
		void Compile(std::unordered_map<unsigned int, std::string>& shaderSources);
	public:
		/// <summary>
		/// 着色器
		/// </summary>
		/// <param name="filepath">着色器文件路径(不含后缀名)</param>
		Shader(const std::string& filepath);
		
		~Shader();

		/// <summary>
		/// 绑定：使用着色器
		/// </summary>
		void Bind() const;

		/// <summary>
		/// 解除绑定：调试时使用
		/// </summary>
		void UnBind() const;

		/// <summary>
		/// 设置 uniform Int 变量
		/// </summary>
		/// <param name="name">变量名</param>
		/// <param name="value">变量值</param>
		void SetInt(const std::string& name, int value);

		/// <summary>
		/// 设置 uniform Int Array 变量
		/// </summary>
		/// <param name="name">变量名</param>
		/// <param name="values">变量值</param>
		/// <param name="count">Array 元素数量</param>
		void SetIntArray(const std::string& name, int* values, uint32_t count);

		/// <summary>
		/// 设置 uniform Float 变量
		/// </summary>
		/// <param name="name">变量名</param>
		/// <param name="value">变量值</param>
		void SetFloat(const std::string& name, float value);

		/// <summary>
		/// 设置 uniform Float2 变量
		/// </summary>
		/// <param name="name">变量名</param>
		/// <param name="value">变量值</param>
		void SetFloat2(const std::string& name, const glm::vec2& value);

		/// <summary>
		/// 设置 uniform Float3 变量
		/// </summary>
		/// <param name="name">变量名</param>
		/// <param name="value">变量值</param>
		void SetFloat3(const std::string& name, const glm::vec3& value);

		/// <summary>
		/// 设置 uniform Float4 变量
		/// </summary>
		/// <param name="name">变量名</param>
		/// <param name="value">变量值</param>
		void SetFloat4(const std::string& name, const glm::vec4& value);

		/// <summary>
		/// 设置 uniform Matrix4 变量
		/// </summary>
		/// <param name="name">变量名</param>
		/// <param name="value">变量值</param>
		void SetMat4(const std::string& name, const glm::mat4& value);

		const std::string GetName() const { return m_Name; }

		// ---- 下列方法：上传 Uniform 变量到 Shader ---- |（变量在 Shader 中的变量名，变量值）

		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	};

	/// <summary>
	/// 着色器库 TODO 可视化查看当前着色器库的所有着色器
	/// </summary>
	class ShaderLibrary
	{
	private:
		std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;		// 着色器 map：着色器名 - 着色器
	public:
		/// <summary>
		/// 添加着色器
		/// </summary>
		/// <param name="name">着色器名</param>
		/// <param name="shader">着色器</param>
		void Add(const std::string& name, const std::shared_ptr<Shader>& shader);

		/// <summary>
		/// 添加着色器
		/// </summary>
		/// <param name="shader">着色器</param>
		void Add(const std::shared_ptr<Shader>& shader);

		/// <summary>
		/// 加载着色器
		/// </summary>
		/// <param name="filepath">文件路径</param>
		/// <returns>着色器</returns>
		std::shared_ptr<Shader> Load(const std::string& filepath);

		/// <summary>
		/// 加载着色器
		/// </summary>
		/// <param name="name">着色器名称</param>
		/// <param name="filepath">文件路径</param>
		/// <returns>着色器</returns>
		std::shared_ptr<Shader> Load(const std::string& name, const std::string& filepath);

		/// <summary>
		/// 返回着色器
		/// </summary>
		/// <param name="name">着色器名称</param>
		/// <returns>着色器</returns>
		std::shared_ptr<Shader> Get(const std::string& name);

		/// <summary>
		/// 着色器是否存在
		/// </summary>
		/// <param name="name">着色器名</param>
		/// <returns>是否存在</returns>
		bool Exists(const std::string& name) const;
	};
}