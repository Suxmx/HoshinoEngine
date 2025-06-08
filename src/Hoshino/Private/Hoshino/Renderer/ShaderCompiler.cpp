#include "Hoshino/Renderer/ShaderCompiler.h"
#include "Hoshino/Renderer/ShaderFactory.h"

#include <filesystem>
namespace Hoshino
{
	std::string ShaderCompiler::m_SlangPath = ".\\Res\\slangc.exe";

	bool ShaderCompiler::CompileShaderToPath(const ShaderCompileDesc& desc)
	{
		// 构建编译命令行
		std::string cmdLine = BuildShaderCommandLine(desc);

		// 添加重定向标准错误到标准输出，方便捕获所有输出
		cmdLine += " 2>&1";

		if(desc.logCmd)
		{
			CORE_TRACE("Compiling shader: {0}", cmdLine);
		}

		// 执行编译命令
		FILE* pipe = _popen(cmdLine.c_str(), "r");
		if (!pipe)
		{
			CORE_ERROR("Failed to start shader compiler process");
			return false;
		}

		// 捕获命令输出
		std::string output;
		char buffer[1024];
		while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
		{
			output += buffer;
		}

		// 检查编译结果
		int status = _pclose(pipe);
		bool success = (status == 0);

		if (success)
		{
			CORE_INFO("Shader compilation successful: {0}", desc.outputPath);

			// 检查是否需要将二进制输出转换为头文件
			if (desc.generateHeader && !desc.outputPath.empty())
			{
				if (!ConvertBinaryToHeader(desc.outputPath, desc.outputPath + ".h", desc.headerVarName))
				{
					CORE_ERROR("Failed to convert binary shader to header file");
					return false;
				}
			}
		}
		else
		{
			// 输出编译错误
			CORE_ERROR("Shader compilation failed: {0}", output);
			return false;
		}
        return true;
	}

	std::string ShaderCompiler::BuildShaderCommandLine(const ShaderCompileDesc& desc)
	{
		std::stringstream cmdSS;
		cmdSS << "" << m_SlangPath << "";

		// 着色器配置文件
		std::string profile;
		switch (desc.shaderType)
		{
		case ShaderType::Vertex:
			profile = "vs";
			break;
		case ShaderType::Pixel:
			profile = "ps";
			break;
		// case ShaderType::Compute:
		// 	profile = "cs";
		// 	break;
		// case ShaderType::Geometry:
		// 	profile = "gs";
		// 	break;
		// case ShaderType::Hull:
		// 	profile = "hs";
		// 	break;
		// case ShaderType::Domain:
		// 	profile = "ds";
		// 	break;
		// case ShaderType::Library:
		// 	profile = "lib";
		// 	break;
		default:
			throw std::runtime_error("不支持的着色器类型");
		}

		// HLSL兼容模式
		cmdSS << " -lang hlsl";
		cmdSS << " -unscoped-enum";

		// 着色器配置文件
		cmdSS << " -profile " << profile << "_" << desc.shaderModel;

		// 目标平台
		switch (desc.compilerType)
		{
		case CompilerType::DXBC:
			cmdSS << " -target dxbc";
			break;
		case CompilerType::DXIL:
			cmdSS << " -target dxil";
			break;
		case CompilerType::SPIRV:
			cmdSS << " -target spirv";
			break;
		}

		// 输出文件
		if (!desc.outputPath.empty())
		{
			cmdSS << " -o " << desc.outputPath;
		}

		// // 宏定义
		// for (const auto& define : desc.defines)
		// {
		// 	cmdSS << " -D " << define;
		// }

		// // 包含目录
		// for (const auto& dir : desc.includeDirs)
		// {
		// 	cmdSS << " -I " << dir;
		// }

		// 优化级别
		cmdSS << " -O" << desc.optimizationLevel;

		// 警告视为错误
		if (desc.warningsAsErrors)
		{
			cmdSS << " -warnings-as-errors";
		}

		// 矩阵布局
		if (desc.matrixRowMajor)
		{
			cmdSS << " -matrix-layout-row-major";
		}
		else
		{
			cmdSS << " -matrix-layout-column-major";
		}

		// SPIRV特定选项
		if (desc.compilerType == CompilerType::SPIRV)
		{
			// 使用源码中的入口点名称
			cmdSS << " -fvk-use-entrypoint-name";

			// 寄存器偏移
			if (!desc.noRegShifts)
			{
				for (int space = 0; space < 4; space++)
				{
					cmdSS << " -fvk-s-shift " << desc.sRegShift << " " << space;
					cmdSS << " -fvk-t-shift " << desc.tRegShift << " " << space;
					cmdSS << " -fvk-b-shift " << desc.bRegShift << " " << space;
					cmdSS << " -fvk-u-shift " << desc.uRegShift << " " << space;
				}
			}
		}

		// 源文件路径（最后添加）
		cmdSS << " " << desc.filePath;

		return cmdSS.str();
	}

	// 辅助函数：将二进制文件转换为C++头文件
	bool ShaderCompiler::ConvertBinaryToHeader(const std::string& binaryPath, const std::string& headerPath,
	                                           const std::string& varName)
	{
		// 读取二进制文件
		std::ifstream binFile(binaryPath, std::ios::binary);
		if (!binFile)
		{
			CORE_ERROR("Failed to open binary file: {0}", binaryPath);
			return false;
		}

		// 获取文件大小
		binFile.seekg(0, std::ios::end);
		size_t fileSize = binFile.tellg();
		binFile.seekg(0, std::ios::beg);

		// 读取文件内容
		std::vector<uint8_t> buffer(fileSize);
		if (fileSize > 0)
		{
			binFile.read(reinterpret_cast<char*>(buffer.data()), fileSize);
		}
		binFile.close();

		// 创建头文件
		std::ofstream header(headerPath);
		if (!header)
		{
			CORE_ERROR("Failed to create header file: {0}", headerPath);
			return false;
		}

		// 写入头文件内容
		std::string actualVarName =
		    varName.empty() ? "g_" + std::filesystem::path(binaryPath).stem().string() + "Shader" : varName;

		// 写入头文件
		header << "// 自动生成的着色器字节码头文件\n";
		header << "// 源文件: " << binaryPath << "\n\n";
		header << "#pragma once\n\n";
		header << "static const unsigned char " << actualVarName << "[] = {\n    ";

		// 写入字节数据
		for (size_t i = 0; i < buffer.size(); ++i)
		{
			header << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(buffer[i]);

			if (i < buffer.size() - 1) header << ", ";

			// 每16个字节换行以提高可读性
			if ((i + 1) % 16 == 0 && i < buffer.size() - 1) header << "\n    ";
		}

		header << "\n};\n\n";
		header << "static const size_t " << actualVarName << "Size = sizeof(" << actualVarName << ");\n";

		return true;
	}
} // namespace Hoshino