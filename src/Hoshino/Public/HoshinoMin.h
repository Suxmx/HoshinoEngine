#pragma once

#include "HoshinoPch.h"
#include "Hoshino/Log.h"
#include "Hoshino/Assert.h"
#include "Hoshino/Macro.h"

namespace Hoshino
{
	template <typename T>
	using Ref = std::shared_ptr<T>;
	template <typename T>
	using Scope = std::unique_ptr<T>;
} // namespace Hoshino