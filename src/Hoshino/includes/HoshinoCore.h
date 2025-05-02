#pragma once
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

#include "Application.h"
#include "Event/Event.h"
#include "Log.h"
#include <glad/glad.h>
#include <iostream>
#include <memory>
