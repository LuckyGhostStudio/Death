#include "lcpch.h"
#include "Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Lucky
{
	Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc) :m_RendererID(0)
	{
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);		// 创建顶点着色器
		const char* source = vertexSrc.c_str();

		glShaderSource(vertexShader, 1, &source, 0);						// 顶点着色器源代码发送到 GL
		glCompileShader(vertexShader);										// 编译顶点着色器

		int isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);		// 获取编译状态

		if (isCompiled == GL_FALSE)
		{
			int maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);			// 获取编译日志信息长度

			std::vector<char> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);	// 获取编译日志信息

			glDeleteShader(vertexShader);											// 删除着色器

			LC_CORE_ERROR("{0}", infoLog.data());									// 失败信息
			LC_CORE_ASSERT(false, "Vertex Shader Compilation Failure!");			// 顶点着色器编译失败

			return;
		}

		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);		// 创建片元着色器
		source = fragmentSrc.c_str();

		glShaderSource(fragmentShader, 1, &source, 0);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);

		if (isCompiled == GL_FALSE)
		{
			int maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);

			LC_CORE_ERROR("{0}", infoLog.data());							// 失败信息
			LC_CORE_ASSERT(false, "Fragment Shader Compilation Failure!");	// 片元着色器编译失败

			return;
		}

		m_RendererID = glCreateProgram();			// 创建程序
		unsigned int program = m_RendererID;
		glAttachShader(program, vertexShader);		// 将顶点着色器添加到程序
		glAttachShader(program, fragmentShader);	// 将片元着色器添加到程序
		glLinkProgram(program);						// 链接程序

		int isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);	// 获取链接状态

		if (isLinked == GL_FALSE)
		{
			int maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			LC_CORE_ERROR("{0}", infoLog.data());			// 失败信息
			LC_CORE_ASSERT(false, "Shader Link Failure!");	// 着色器链接失败

			return;
		}

		// 分离着色器
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);	// 删除着色器程序
	}

	void Shader::Bind() const
	{
		glUseProgram(m_RendererID);		// 使用着色器程序
	}

	void Shader::UnBind() const
	{
		glUseProgram(0);
	}

	// ---- 下列方法：上传 Uniform 变量到 Shader ----

	void Shader::UploadUniformInt(const std::string& name, int value)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());	// 获取 Uniform 变量位置
		glUniform1i(location, value);										// 设置 Uniform 变量（位置，变量个数，是否转置，变量地址）
	}

	void Shader::UploadUniformFloat(const std::string& name, float value)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void Shader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void Shader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void Shader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void Shader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}