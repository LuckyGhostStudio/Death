#pragma once

#include "Lucky/Core/Core.h"

namespace Lucky
{
    /// <summary>
    /// 帧缓冲区纹理格式
    /// </summary>
    enum class FramebufferTextureFormat
    {
        None = 0,

        RGBA8,                      // 颜色 RGBA
        RED_INTEGER,                // 红色整型

        DEFPTH24STENCIL8,           // 深度模板

        Depth = DEFPTH24STENCIL8    // 默认值
    };

    /// <summary>
    /// 帧缓冲区纹理规范
    /// </summary>
    struct FramebufferTextureSpecification
    {
        FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;    // 纹理格式

        FramebufferTextureSpecification() = default;
        FramebufferTextureSpecification(FramebufferTextureFormat format) :TextureFormat(format) {}
    };

    /// <summary>
    /// 帧缓冲区附件规范
    /// </summary>
    struct FramebufferAttachmentSpecification
    {
        std::vector<FramebufferTextureSpecification> Attachments;   // 附件列表

        FramebufferAttachmentSpecification() = default;
        FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
            :Attachments(attachments) {}
    };

    /// <summary>
    /// 帧缓冲区规范
    /// </summary>
    struct FramebufferSpecification
    {
        uint32_t Width;     // 帧缓冲区宽
        uint32_t Height;    // 帧缓冲区高

        FramebufferAttachmentSpecification Attachments; // 帧缓冲区所有附件

        uint32_t Samples = 1;   // 采样数

        bool SwapChainTarget = false;   // 是否要渲染到屏幕
    };

    /// <summary>
    /// 帧缓冲区
    /// </summary>
    class Framebuffer
    {
    private:
        uint32_t m_RendererID = 0;                  // 帧缓冲区 ID
        
        FramebufferSpecification m_Specification;   // 帧缓冲区规范

        std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecs;                    // 颜色缓冲区规范列表
        FramebufferTextureSpecification m_DepthAttachmentSpec = FramebufferTextureFormat::None; // 深度缓冲区规范

        std::vector<uint32_t> m_ColorAttachments;   // 颜色缓冲区 ID 列表
        uint32_t m_DepthAttachment = 0;             // 深度缓冲区 ID
    public:
        /// <summary>
        /// 帧缓冲区
        /// </summary>
        /// <param name="spec">帧缓冲区规范</param>
        Framebuffer(const FramebufferSpecification& spec);

        ~Framebuffer();

        /// <summary>
        /// 调整大小：使无效
        /// </summary>
        void Invalidate();

        /// <summary>
        /// 绑定帧缓冲区
        /// </summary>
        void Bind() const;

        /// <summary>
        /// 解除绑定
        /// </summary>
        void Unbind() const;

        /// <summary>
        /// 重置帧缓冲区大小
        /// </summary>
        /// <param name="width">宽</param>
        /// <param name="height">高</param>
        void Resize(uint32_t width, uint32_t height);

        /// <summary>
        /// 读取像素
        /// </summary>
        /// <param name="attachmentIndex">颜色缓冲区 id</param>
        /// <param name="x">x坐标</param>
        /// <param name="y">y坐标</param>
        /// <returns>像素数据：输出到 attachmentIndex 颜色缓冲区的数据</returns>
        int GetPixel(uint32_t attachmentIndex, int x, int y);

        /// <summary>
        /// 清除帧缓冲区附件
        /// </summary>
        /// <param name="attachmentIndex">颜色缓冲区 id</param>
        /// <param name="value">清除值</param>
        void ClearAttachment(uint32_t attachmentIndex, int value);

        /// <summary>
        /// 返回颜色缓冲区 ID
        /// </summary>
        /// <param name="index">ID 列表索引</param>
        /// <returns></returns>
        uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const
        {
            LC_CORE_ASSERT(index < m_ColorAttachments.size(), "index 越界！");

            return m_ColorAttachments[index];
        }

        /// <summary>
        /// 返回帧缓冲区规范
        /// </summary>
        /// <returns>帧缓冲区规范</returns>
        const FramebufferSpecification& GetSpecification() const { return m_Specification; }
    };
}