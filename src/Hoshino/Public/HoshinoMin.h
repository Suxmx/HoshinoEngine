#pragma once

#include "HoshinoPch.h"
#include "Hoshino/Log.h"
#include "Hoshino/Assert.h"
#include "Hoshino/Macro.h"

#include <nvrhi/common/resource.h>

namespace Hoshino
{
	template <typename T>
	using Ref = std::shared_ptr<T>;
	template <typename T>
	using Scope = std::unique_ptr<T>;
	template <typename T, typename... Args>
	Ref<T> CreateRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
	template <typename T, typename... Args>
	Scope<T> CreateScope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using RefCountPtr = nvrhi::RefCountPtr<T>;
} // namespace Hoshino