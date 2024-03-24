#pragma once

#ifdef LC_DEBUG
	#define LC_ENABLE_ASSERTS
#endif

#ifdef LC_ENABLE_ASSERTS	// 启用断言
	// 断言：x为假 则显示ERROR日志信息 并中断调试
	#define LC_ASSERT(x, ...) { if(!(x)) { LC_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	// 断言：x为假 则显示ERROR日志信息 并中断调试
	#define LC_CORE_ASSERT(x, ...) { if(!(x)) { LC_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else	// 禁用断言
	#define LC_ASSERT(x, ...)
	#define LC_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)		// 1左移x位

#define LC_BIND_EVENT_FUNC(func) std::bind(&func, this, std::placeholders::_1)	// 绑定事件函数 返回函数对象