#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"

namespace Lucky
{
	/// <summary>
	/// 日志
	/// </summary>
	class Log
	{
	public:
		/// <summary>
		/// 初始化
		/// </summary>
		static void Init();

		/// <summary>
		/// 返回内核 Log
		/// </summary>
		/// <returns>内核 Log</returns>
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }

		/// <summary>
		/// 返回客户端 Log
		/// </summary>
		/// <returns>客户端 Log</returns>
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;		// 内核 Log
		static std::shared_ptr<spdlog::logger> s_ClientLogger;		// 客户端 Log
	};
}

// 内核 Log
#define LC_CORE_TRACE(...)	::Lucky::Log::GetCoreLogger()->trace(__VA_ARGS__)		// 提示
#define LC_CORE_INFO(...)	::Lucky::Log::GetCoreLogger()->info(__VA_ARGS__)		// 信息
#define LC_CORE_WARN(...)	::Lucky::Log::GetCoreLogger()->warn(__VA_ARGS__)		// 警告
#define LC_CORE_ERROR(...)	::Lucky::Log::GetCoreLogger()->error(__VA_ARGS__)		// 错误
#define LC_CORE_FATAL(...)	::Lucky::Log::GetCoreLogger()->fatal(__VA_ARGS__)		// 严重错误

// 客户端 Log
#define LC_TRACE(...)		::Lucky::Log::GetClientLogger()->trace(__VA_ARGS__)		// 提示
#define LC_INFO(...)		::Lucky::Log::GetClientLogger()->info(__VA_ARGS__)		// 信息
#define LC_WARN(...)		::Lucky::Log::GetClientLogger()->warn(__VA_ARGS__)		// 警告
#define LC_ERROR(...)		::Lucky::Log::GetClientLogger()->error(__VA_ARGS__)		// 错误
#define LC_FATAL(...)		::Lucky::Log::GetClientLogger()->fatal(__VA_ARGS__)		// 严重错误