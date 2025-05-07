#pragma once
#include "Hoshino/Macro.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Hoshino
{
	class HOSHINO_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger()
		{
			return s_CoreLogger;
		}
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger()
		{
			return s_ClientLogger;
		}

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
} // namespace Hoshino

#define CORE_ERROR(...) ::Hoshino::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_WARN(...)  ::Hoshino::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_INFO(...)  ::Hoshino::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_TRACE(...) ::Hoshino::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_FATAL(...) ::Hoshino::Log::GetCoreLogger()->fatal(__VA_ARGS__)
#ifdef HOSHINO_DEBUG_LOG
	#define CORE_DEBUG(...) ::Hoshino::Log::GetCoreLogger()->trace(__VA_ARGS__)
#else
	#define CORE_DEBUG(...)
#endif

#define APP_ERROR(...)  ::Hoshino::Log::GetClientLogger()->error(__VA_ARGS__)
#define APP_WARN(...)   ::Hoshino::Log::GetClientLogger()->warn(__VA_ARGS__)
#define APP_INFO(...)   ::Hoshino::Log::GetClientLogger()->info(__VA_ARGS__)
#define APP_TRACE(...)  ::Hoshino::Log::GetClientLogger()->trace(__VA_ARGS__)
#define APP_FATAL(...)  ::Hoshino::Log::GetClientLogger()->fatal(__VA_ARGS__)
#ifdef HOSHINO_DEBUG_LOG
	#define APP_DEBUG(...) ::Hoshino::Log::GetClientLogger()->trace(__VA_ARGS__)
#else
	#define APP_DEBUG(...)
#endif
