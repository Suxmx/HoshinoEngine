#include "Hoshino/Renderer/ShaderFactory.h"
#include "Hoshino/Vfs/VfsUtils.h"

namespace Hoshino
{
	nvrhi::ShaderHandle ShaderFactory::CreateShaderFromBinary(const char* fileRelativePath,
	                                                          const char* entryName, ShaderType shaderType)
	{
		nvrhi::ShaderDesc shaderDesc;
		shaderDesc.setShaderType((nvrhi::ShaderType)shaderType);
		shaderDesc.debugName = fileRelativePath;

		return CreateShaderFromBinary(fileRelativePath, shaderDesc);
	}

	nvrhi::ShaderHandle ShaderFactory::CreateShaderFromBinary(const char* fileRelativePath,
	                                                          nvrhi::ShaderDesc shaderDesc)
	{
		std::string filePath = (m_ShaderCachePath / fileRelativePath).string();
		if (!Vfs::FileExists(filePath))
		{
			CORE_ERROR("Shader binary file does not exist: {0}", fileRelativePath);
			return nullptr;
		}
		auto blob = Vfs::ReadFile(filePath);
		if (!blob || Vfs::IBlob::isEmpty(blob.get()))
		{
			CORE_ERROR("Failed to read shader binary file: {0}", filePath);
			return nullptr;
		}
		return InternalCreateShaderFromBinary(blob, shaderDesc);
	}

	nvrhi::ShaderHandle ShaderFactory::CreateShaderFromSource(const ShaderCompileDesc& compileDesc,
	                                                          const char* entryName)
	{
		nvrhi::ShaderDesc shaderDesc;
		shaderDesc.setShaderType((nvrhi::ShaderType)compileDesc.shaderType);
		shaderDesc.debugName = compileDesc.outputPath;
		return CreateShaderFromSource(compileDesc, shaderDesc);
	}

	nvrhi::ShaderHandle ShaderFactory::CreateShaderFromSource(const ShaderCompileDesc& compileDesc,
	                                                          nvrhi::ShaderDesc shaderDesc)
	{
		std::string sourceFile = compileDesc.filePath;
		if (!Vfs::FileExists(sourceFile))
		{
			CORE_ERROR("Shader source file does not exist: {0}", sourceFile);
			return nullptr;
		}
		if (!ShaderCompiler::CompileShader(compileDesc))
		{
			CORE_ERROR("Failed to compile shader: {0} due to compile error", sourceFile);
			return nullptr;
		}
		auto blob = Vfs::ReadFile(compileDesc.outputPath);
		if (!blob || Vfs::IBlob::isEmpty(blob.get()))
		{
			CORE_ERROR("Failed to read shader binary file: {0}", compileDesc.outputPath);
			return nullptr;
		}
		return InternalCreateShaderFromBinary(blob, shaderDesc);
	}

	nvrhi::ShaderHandle ShaderFactory::InternalCreateShaderFromBinary(Ref<Vfs::IBlob> blob,
	                                                                  const nvrhi::ShaderDesc& shaderDesc)
	{
		if (!blob || Vfs::IBlob::isEmpty(blob.get()))
		{
			CORE_ERROR("Invalid shader binary blob");
			return nullptr;
		}
		return m_Device->createShader(shaderDesc, blob->data(), blob->size());
	}

} // namespace Hoshino