#pragma once
#include "HoshinoMin.h"
#include <nvrhi/nvrhi.h>
namespace Hoshino
{
	enum class ShaderType
	{
		Vertex = (int)nvrhi::ShaderType::Vertex,
		Pixel = (int)nvrhi::ShaderType::Pixel,
	};
}