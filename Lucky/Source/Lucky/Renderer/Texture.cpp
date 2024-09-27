#include "lcpch.h"
#include "Texture.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace Lucky
{
	Texture2D::Texture2D(uint32_t width, uint32_t height) :m_Width(width), m_Height(height)
	{
		m_InternalFormat = GL_RGBA8;	// 内部格式
		m_DataFormat = GL_RGBA;			// 数据格式
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);		// 创建 2D 纹理
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);	// 存储 2D 纹理 - - 内部格式 - -
		
		// 设置纹理参数
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// 缩小过滤器 线性插值
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	// 放大过滤器 最近滤波
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);		// x 超过 0-1 重复
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);		// y 超过 0-1 重复
	}

	Texture2D::Texture2D(const std::string& path)
		: m_Path(path)
	{
		int width, height, channels;			// 宽 高 通道数
		
		stbi_set_flip_vertically_on_load(1);	// 垂直翻转

		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);	// 加载图片

		LC_CORE_ASSERT(data, "Failed to load image!");		// 加载失败

		m_Width = width;
		m_Height = height;

		if (channels == 4) {				// 4 颜色通道
			m_InternalFormat = GL_RGBA8;	// 每个通道 8 位
			m_DataFormat = GL_RGBA;
		}
		else if (channels == 3) {			// 3 颜色通道
			m_InternalFormat = GL_RGB8;		// 每个通道 8 位
			m_DataFormat = GL_RGB;
		}

		LC_CORE_ASSERT(m_InternalFormat & m_DataFormat, "Format not supported!");	// 不支持此格式

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);							// 创建 2D 纹理
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);	// 存储 2D 纹理 - - 内部格式 - -

		// 设置纹理参数 TODO 设置自定义资产格式后 可在编辑器中设置资产参数
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// 缩小过滤器 线性插值
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	// 放大过滤器 最近滤波

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);		// x 超过0-1 重复
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);		// y 超过0-1 重复
		//glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// x 超过0-1 延伸
		//glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	// y 超过0-1 延伸


		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);	// 生成纹理到 GPU

		stbi_image_free(data);	// 释放 data
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_RendererID);		// 删除纹理
	}

	void Texture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;															// 每个像素字节数
		LC_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");						// 纹理数据不完整
		
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);	// 生成纹理到GPU
	}

	void Texture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);	// 绑定纹理到 slot 槽位
	}
}