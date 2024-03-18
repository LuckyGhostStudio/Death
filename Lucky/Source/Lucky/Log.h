#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"

namespace Lucky
{
	/// <summary>
	/// ��־
	/// </summary>
	class Log
	{
	public:
		/// <summary>
		/// ��ʼ��
		/// </summary>
		static void Init();

		/// <summary>
		/// �����ں� Log
		/// </summary>
		/// <returns>�ں� Log</returns>
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }

		/// <summary>
		/// ���ؿͻ��� Log
		/// </summary>
		/// <returns>�ͻ��� Log</returns>
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;		// �ں� Log
		static std::shared_ptr<spdlog::logger> s_ClientLogger;		// �ͻ��� Log
	};
}

// �ں� Log
#define LC_CORE_TRACE(...)	::Lucky::Log::GetCoreLogger()->trace(__VA_ARGS__)		// ��ʾ
#define LC_CORE_INFO(...)	::Lucky::Log::GetCoreLogger()->info(__VA_ARGS__)		// ��Ϣ
#define LC_CORE_WARN(...)	::Lucky::Log::GetCoreLogger()->warn(__VA_ARGS__)		// ����
#define LC_CORE_ERROR(...)	::Lucky::Log::GetCoreLogger()->error(__VA_ARGS__)		// ����
#define LC_CORE_FATAL(...)	::Lucky::Log::GetCoreLogger()->fatal(__VA_ARGS__)		// ���ش���

// �ͻ��� Log
#define LC_TRACE(...)		::Lucky::Log::GetClientLogger()->trace(__VA_ARGS__)		//��ʾ
#define LC_INFO(...)		::Lucky::Log::GetClientLogger()->info(__VA_ARGS__)		//��Ϣ
#define LC_WARN(...)		::Lucky::Log::GetClientLogger()->warn(__VA_ARGS__)		//����
#define LC_ERROR(...)		::Lucky::Log::GetClientLogger()->error(__VA_ARGS__)		//����
#define LC_FATAL(...)		::Lucky::Log::GetClientLogger()->fatal(__VA_ARGS__)		//���ش���