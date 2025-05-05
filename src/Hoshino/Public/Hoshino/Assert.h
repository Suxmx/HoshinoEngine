#include "Hoshino/Log.h"
#ifdef HOSHINO_ASSERT
#define CORE_ASSERT(x, ...)          \
	{                                \
		if (!(x))                    \
		{                            \
			CORE_ERROR(__VA_ARGS__); \
			__debugbreak();          \
		}                            \
	}
#else
#define APP_ASSERT(x, ...)          \
	{                               \
		if (!(x))                   \
		{                           \
			APP_ERROR(__VA_ARGS__); \
			__debugbreak();         \
		}                           \
	}
#endif // HOSHINO_ASSERT