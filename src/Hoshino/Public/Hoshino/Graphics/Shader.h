#pragma once
#include "HoshinoMin.h"

namespace Hoshino
{
	class HOSHINO_API Shader
	{
	public:
		Shader(const std::string vertexShaderSrc, const std::string fragmentShaderSrc) :
		    m_VertexShaderSrc(vertexShaderSrc), m_FragmentShaderSrc(fragmentShaderSrc)
		{}
		Shader(const std::string& filePath) : m_FilePath(filePath)
		{
		}
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void UploadUniformInt(const std::string& name, int value) = 0;

		virtual void UploadUniformFloat(const std::string& name, float value) = 0;
		virtual void UploadUniformFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void UploadUniformFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void UploadUniformFloat4(const std::string& name, const glm::vec4& value) = 0;

		virtual void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) = 0;
		virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) = 0;
		static Ref<Shader> Create(const std::string& vertexShaderSrc,
		                                      const std::string& fragmentShaderSrc);
		static Ref<Shader> CreateFromFile(const std::string& vertexFile,const std::string& fragmentFile);

	protected:
		std::string m_VertexShaderSrc;
		std::string m_FragmentShaderSrc;
		std::string m_FilePath;
	};
} // namespace Hoshino