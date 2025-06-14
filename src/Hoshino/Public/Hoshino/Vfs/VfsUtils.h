#pragma once
#include "HoshinoMin.h"
#include <filesystem>

namespace Hoshino::Vfs
{
    class IBlob;
    // 检查文件是否存在
	bool FileExists(const std::filesystem::path& filePath);

    // 检查目录是否存在
	bool DirectoryExists(const std::filesystem::path& filePath);

	// 创建目录
	bool CreateDirectory(const std::filesystem::path& filePath, bool recursive = false);

	Ref<IBlob> ReadFile(const std::filesystem::path& filePath);
} // namespace Hoshino::Vfs