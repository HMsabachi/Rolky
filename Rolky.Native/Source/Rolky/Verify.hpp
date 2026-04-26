#pragma once

#ifdef __clang__
	#define ROLKY_HAS_SOURCE_LOCATION 0
#else
	#define ROLKY_HAS_SOURCE_LOCATION __has_include(<source_location>)
#endif

#if ROLKY_HAS_SOURCE_LOCATION
	#include <source_location>

	#define ROLKY_SOURCE_LOCATION                                        \
		std::source_location location = std::source_location::current(); \
		const char* file = location.file_name();                         \
		int line = location.line()
#else
	#define ROLKY_SOURCE_LOCATION    \
		const char* file = __FILE__; \
		int line = __LINE__
#endif

#if defined(__clang__)
	#define ROLKY_DEBUG_BREAK __builtin_debugtrap()
#elif defined(_MSC_VER)
	#define ROLKY_DEBUG_BREAK __debugbreak()
#else
	#define ROLKY_DEBUG_BREAK
#endif

#define ROLKY_VERIFY(expr)                                                                                                \
	{                                                                                                                     \
		if (!(expr))                                                                                                      \
		{                                                                                                                 \
			ROLKY_SOURCE_LOCATION;                                                                                        \
			std::cerr << "[Rolky.Native]: Assert Failed! Expression: " << #expr << " at " << file << ":" << line << "\n"; \
			ROLKY_DEBUG_BREAK;                                                                                            \
		}                                                                                                                 \
	}