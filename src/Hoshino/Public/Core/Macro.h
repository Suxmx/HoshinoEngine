// Macros
#ifdef _WIN32
#ifdef HOSHINO_EXPORTS // 定义在 DLL 编译时
#define HOSHINO_API __declspec(dllexport)
#else
#define HOSHINO_API __declspec(dllimport)
#endif
#else
#define HOSHINO_API // 非 Windows 平台留空
#endif

#define BIT(x) (1 << x)