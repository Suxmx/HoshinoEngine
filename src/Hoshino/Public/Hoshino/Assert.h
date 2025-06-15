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
#define APP_ASSERT(x, ...)          \
{                               \
if (!(x))                   \
{                           \
APP_ERROR(__VA_ARGS__); \
__debugbreak();         \
}                           \
}
#else
#define CORE_ASSERT(x, ...)
#define APP_ASSERT(x, ...)
#endif // HOSHINO_ASSERT