#pragma once

#include <string>
#include <optional>

namespace Lucky
{
    /// <summary>
    /// 文件对话框
    /// </summary>
    class FileDialogs
    {
    public:
        /// <summary>
        /// 打开文件：取消则返回空
        /// </summary>
        /// <param name="filter">文件过滤器：文件类型</param>
        /// <returns>文件路径</returns>
        static std::optional<std::string> OpenFile(const char* filter);

        /// <summary>
        /// 保存文件：取消则返回空
        /// </summary>
        /// <param name="filter">文件过滤器：文件类型</param>
        /// <returns>文件路径</returns>
        static std::optional<std::string> SaveFile(const char* filter);
    };
}