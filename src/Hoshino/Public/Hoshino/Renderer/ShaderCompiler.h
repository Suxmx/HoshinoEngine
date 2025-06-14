#pragma once
#include "HoshinoMin.h"

namespace Hoshino
{
	enum class ShaderType;
	struct ShaderCompileDesc;

	class ShaderCompiler
	{
	public:
		enum CompilerType
		{
			DXBC,
			DXIL,
			SPIRV
		};
		static bool CompileShader(const ShaderCompileDesc& desc);

	private:
		static std::string BuildShaderCommandLine(const ShaderCompileDesc& desc);
		static bool ConvertBinaryToHeader(const std::string& binaryPath, const std::string& headerPath,
		                           const std::string& varName);
		inline static void SetSlangPath(const char* path)
		{
			m_SlangPath = path;
		}

		static std::string m_SlangPath;
	};

	struct ShaderCompileDesc
	{
		// bool slang = true;
		// bool slangHlsl = true;
		bool matrixRowMajor = false;
		// path
		std::string filePath;
		std::string outputPath;
		// type
		ShaderCompiler::CompilerType compilerType;
		ShaderType shaderType;

		const char* shaderModel = "6_5";
		const char* vulkanVersion = "1.3";
		// debug
		bool pdb = false;
		bool embedPdb = false;
		bool warningsAsErrors = false;
		bool logCmd = false;
		// 寄存器偏移
		bool noRegShifts = false;
		uint32_t sRegShift = 100;
		uint32_t tRegShift = 200;
		uint32_t bRegShift = 300;
		uint32_t uRegShift = 400;
		uint32_t optimizationLevel = 3; // 0: no optimization, 1: basic, 2: advanced, 3: full

		bool generateHeader = false;    // 是否生成头文件
		std::string headerVarName = ""; // 头文件中变量名（为空时自动生成）
	};
} // namespace Hoshino