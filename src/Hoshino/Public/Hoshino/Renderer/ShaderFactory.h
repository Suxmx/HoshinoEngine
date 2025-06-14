#include "HoshinoMin.h"
#include "Hoshino/Renderer/ShaderCompiler.h"
#include "Hoshino/Renderer/Shader.h"
#include "Hoshino/Vfs/Blob.h"
#include <nvrhi/nvrhi.h>

#include <filesystem>
namespace Hoshino
{
	class ShaderFactory
	{
	public:
		ShaderFactory(nvrhi::DeviceHandle device, std::filesystem::path path = ".\\CompiledShader") :
		    m_Device(device), m_ShaderCachePath(path)
		{}
		ShaderFactory(nvrhi::DeviceHandle device, std::filesystem::path cachePath,
		              std::filesystem::path sourcePath) :
		    m_Device(device), m_ShaderCachePath(cachePath), m_ShaderSourcePath(sourcePath)
		{}
		// Binary
		nvrhi::ShaderHandle CreateShaderFromBinary(const char* fileRelativePath, const char* entryName,
		                                           ShaderType shaderType);
		nvrhi::ShaderHandle CreateShaderFromBinary(const char* fileRelativePath,nvrhi::ShaderDesc shaderDesc);

		// Source
		nvrhi::ShaderHandle CreateShaderFromSource(const ShaderCompileDesc& compileDesc,
		                                           const char* entryName);
		nvrhi::ShaderHandle CreateShaderFromSource(const ShaderCompileDesc& compileDesc,
		                                           nvrhi::ShaderDesc shaderDesc);

	private:
		nvrhi::DeviceHandle m_Device;
		std::filesystem::path m_ShaderCachePath;
		std::filesystem::path m_ShaderSourcePath;

		nvrhi::ShaderHandle InternalCreateShaderFromBinary(Ref<Vfs::IBlob>,const nvrhi::ShaderDesc& shaderDesc);
	};
} // namespace Hoshino