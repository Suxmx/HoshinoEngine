#include "Hoshino/Log.h"

// 定义静态成员变量
std::shared_ptr<spdlog::logger> Hoshino::Log::s_CoreLogger;
std::shared_ptr<spdlog::logger> Hoshino::Log::s_ClientLogger;

void Hoshino::Log::Init()
{
	// 创建核心日志器并设置级别
	s_CoreLogger = spdlog::stdout_color_mt("Hoshino");
	s_CoreLogger->set_level(spdlog::level::trace);
	// 自定义级别名称（将 critical 显示为 FATAL）
	auto core_formatter = std::make_unique<spdlog::pattern_formatter>();
	core_formatter->set_pattern("%^[%T] %n: %v%$");
	// core_formatter->set_levels(core_level_names);
	s_CoreLogger->set_formatter(std::move(core_formatter));
	// 同理设置客户端日志器
	s_ClientLogger = spdlog::stdout_color_mt("Client");
	s_ClientLogger->set_level(spdlog::level::trace);
	auto client_formatter = std::make_unique<spdlog::pattern_formatter>();
	client_formatter->set_pattern("%^[%T] %n: %v%$");
	// client_formatter->set_levels(client_level_names);
	s_ClientLogger->set_formatter(std::move(client_formatter));
}