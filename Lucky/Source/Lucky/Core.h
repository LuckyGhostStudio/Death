#pragma once

#ifdef LC_DEBUG
	#define LC_ENABLE_ASSERTS
#endif

#ifdef LC_ENABLE_ASSERTS	// ���ö���
	// ���ԣ�xΪ�� ����ʾERROR��־��Ϣ ���жϵ���
	#define LC_ASSERT(x, ...) { if(!(x)) { LC_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	// ���ԣ�xΪ�� ����ʾERROR��־��Ϣ ���жϵ���
	#define LC_CORE_ASSERT(x, ...) { if(!(x)) { LC_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else	// ���ö���
	#define LC_ASSERT(x, ...)
	#define LC_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)		// 1����xλ

#define LC_BIND_EVENT_FUNC(func) std::bind(&func, this, std::placeholders::_1)	// ���¼����� ���غ�������