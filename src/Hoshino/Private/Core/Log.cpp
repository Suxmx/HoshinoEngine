#include "Hoshino/Log.h"

// 定义静态成员变量
std::shared_ptr<spdlog::logger> Hoshino::Log::s_CoreLogger;
std::shared_ptr<spdlog::logger> Hoshino::Log::s_ClientLogger;

void Hoshino::Log::Init()
{
    spdlog::set_pattern("%^[%T] %n: %v%$"); // 设置日志格式
    s_CoreLogger = spdlog::stdout_color_mt("HoshinoCore"); // 创建核心日志器
    s_CoreLogger->set_level(spdlog::level::trace); // 设置日志级别
    s_ClientLogger = spdlog::stdout_color_mt("APP"); // 创建客户端日志器
    s_ClientLogger->set_level(spdlog::level::trace); // 设置日志级别
}