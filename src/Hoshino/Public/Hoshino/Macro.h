//DLL
#pragma once
#define HOSHINO_API
// #ifdef HOSHINO_STATIC_BUILD
// 	#define HOSHINO_API
// #else
// 	#ifdef _WIN32
// 		#ifdef HOSHINO_EXPORTS  // 定义在 DLL 编译时
// 			#define HOSHINO_API __declspec(dllexport)
// 		#else
// 			#define HOSHINO_API __declspec(dllimport)
// 		#endif
// 	#else
// 		#define HOSHINO_API // 非 Windows 平台留空
// 	#endif
// #endif

//BIT
#define BIT(x) (1 << x)

#define BIND_CLASS_FN(x) std::bind(&x,this,std::placeholders::_1)