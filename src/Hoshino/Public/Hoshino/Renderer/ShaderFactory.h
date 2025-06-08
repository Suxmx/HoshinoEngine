#include "HoshinoMin.h"

#include <nvrhi/nvrhi.h>

namespace Hoshino
{
	enum class ShaderType
	{
		Vertex = (int)nvrhi::ShaderType::Vertex,
		Pixel = (int)nvrhi::ShaderType::Pixel,
	};
	class ShaderFactory
	{
	public:
		nvrhi::ShaderHandle CreateShaderFromBinary(const char* filePath, const char* entryName);
	};
} // namespace Hoshino