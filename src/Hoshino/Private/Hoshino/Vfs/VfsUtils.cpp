#include "Hoshino/Vfs/VfsUtils.h"
#include "Hoshino/Vfs/Blob.h"
#include <limits>
namespace fs = std::filesystem;

namespace Hoshino::Vfs
{
	bool FileExists(const std::filesystem::path& filePath)
	{
		return fs::exists(filePath) && fs::is_regular_file(filePath);
	}

	bool DirectoryExists(const std::filesystem::path& dirPath)
	{
		return fs::exists(dirPath) && fs::is_directory(dirPath);
	}

	bool CreateDirectory(const std::filesystem::path& dirPath, bool recursive)
	{
		if (DirectoryExists(dirPath))
		{
			return true; // 目录已存在
		}

		try
		{
			if (recursive)
			{
				fs::create_directories(dirPath); // 创建多级目录
			}
			else
			{
				fs::create_directory(dirPath); // 创建单级目录
			}
			return true;
		}
		catch (const fs::filesystem_error& e)
		{
			CORE_ERROR("Failed to create directory: {0}", e.what());
			return false;
		}
	}
	
	Ref<IBlob> ReadFile(const std::filesystem::path& filePath)
	{
		if(!FileExists(filePath))
		{
			CORE_ERROR("File does not exist: {0}", filePath.string());
			return nullptr;
		}
		std::ifstream file(filePath, std::ios::binary);
		if (!file.is_open())
		{
			CORE_ERROR("Failed to open file: {0}", filePath.string());
			return nullptr;
		}
		// 获取大小 防止溢出
		file.seekg(0, std::ios::end);
		size_t size = file.tellg();
		file.seekg(0, std::ios::beg);

		if (size > (std::numeric_limits<size_t>::max)())
		{
			CORE_ERROR("File size exceeds maximum limit: {0}", filePath.string());
			return nullptr;
		}

		char* data = static_cast<char*>(malloc(size));
		file.read(data,size);

		CORE_ASSERT(data != nullptr, "Failed to allocate memory for file: {0}", filePath.string());
		CORE_ASSERT(file.good(), "File stream is not good: {0}", filePath.string());
		return CreateRef<Blob>(data, size);
	}

} // namespace Hoshino::Vfs