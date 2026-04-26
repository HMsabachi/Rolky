#pragma once

#ifdef _WIN32
	#define ROLKY_CALLTYPE __cdecl

	#ifdef _WCHAR_T_DEFINED
		#define ROLKY_STR(s) L##s
		#define ROLKY_WIDE_CHARS 1

		using CharType = wchar_t;
	#else
		#define ROLKY_STR(s) s
		#define ROLKY_WIDE_CHARS 0

		using CharType = unsigned short;
	#endif
#else
	#define ROLKY_CALLTYPE
	#define ROLKY_STR(s) s
	#define ROLKY_WIDE_CHARS 0

	using CharType = char;
#endif
