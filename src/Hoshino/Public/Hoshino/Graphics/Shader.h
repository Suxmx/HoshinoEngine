#pragma once
#include <string>

namespace Hoshino
{
	class HOSHINO_API Shader
	{
	public:
		Shader(const std::string vertexShaderSrc, const std::string fragmentShaderSrc) :
		    m_VertexShaderSrc(vertexShaderSrc), m_FragmentShaderSrc(fragmentShaderSrc)
		{}
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
        static Shader* Create(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc);

	protected:
		std::string m_VertexShaderSrc;
		std::string m_FragmentShaderSrc;
	};
} // namespace Hoshino