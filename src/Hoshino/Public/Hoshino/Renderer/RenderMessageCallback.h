#pragma once
#include "HoshinoMin.h"
#include <nvrhi/nvrhi.h>
namespace Hoshino
{

	class RenderMsgCb : nvrhi::IMessageCallback
	{
		void message(nvrhi::MessageSeverity severity, const char* messageText) override
		{
			switch (severity)
			{
			case nvrhi::MessageSeverity::Info:
				CORE_INFO("[RenderMsg]:{0}", messageText);
				break;
			case nvrhi::MessageSeverity::Warning:
				CORE_WARN("[RenderMsg]:{0}", messageText);
				break;
			case nvrhi::MessageSeverity::Error:
				CORE_ERROR("[RenderMsg]:{0}", messageText);
				break;
			case nvrhi::MessageSeverity::Fatal:
				CORE_ERROR("[RenderMsgFatal]:{0}", messageText);
				break;
			}
		}
	};
} // namespace Hoshino