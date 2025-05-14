#include "Hoshino/Graphics/Shader.h"
#include "HoshinoPch.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Hoshino/Log.h"
#include "Hoshino/Assert.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Hoshino
{
	const std::string SHADER_FILE_PATH = "Res/Shader/";
	Ref<Shader> Shader::Create(const std::string& vertexShaderSrc,
	                                       const std::string& fragmentShaderSrc)
	{
		return std::make_shared<OpenGLShader>(vertexShaderSrc, fragmentShaderSrc);
	}

	static std::string EnsureGlslExtension(const std::string& filename)
	{
		// if (filename.size() < 5 || filename.substr(filename.size() - 5) != ".glsl")
		// 	return filename + ".glsl";
		return filename;
	}

	Ref<Shader> Shader::CreateFromFile(const std::string& vertexFile, const std::string& fragmentFile)
	{
		std::string realVertexPath = SHADER_FILE_PATH + EnsureGlslExtension(vertexFile);
		std::string realFragPath = SHADER_FILE_PATH + EnsureGlslExtension(fragmentFile);
		// Read vertex shader file
		std::string vertexShaderSrc;
		std::ifstream vertexShaderFile(realVertexPath);

		if (vertexShaderFile.is_open())
		{
			std::stringstream vertexStream;
			vertexStream << vertexShaderFile.rdbuf();
			vertexShaderFile.close();
			vertexShaderSrc = vertexStream.str();
		}
		else
		{
			CORE_ASSERT(false, "Failed to read vertex shader file {0}!",realVertexPath);
		}

		// Read fragment shader file
		std::string fragmentShaderSrc;
		std::ifstream fragmentShaderFile(realFragPath);

		if (fragmentShaderFile.is_open())
		{
			std::stringstream fragmentStream;
			fragmentStream << fragmentShaderFile.rdbuf();
			fragmentShaderFile.close();
			fragmentShaderSrc = fragmentStream.str();
		}
		else
		{
			CORE_ASSERT(false, "Failed to read fragment shader file {0}!",realFragPath);
		}
		return Create(vertexShaderSrc, fragmentShaderSrc);
	}
	OpenGLShader::OpenGLShader(const std::string vertexShaderSrc, const std::string fragmentShaderSrc) :
	    Shader(vertexShaderSrc, fragmentShaderSrc)
	{
		// 顶点着色器
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// 0代表凭\0结尾
		const GLchar* source = vertexShaderSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		// 编译失败
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(vertexShader);

			CORE_ERROR("{0}", infoLog.data());
			CORE_ASSERT(false, "Vertex shader compilation failure!");
			return;
		}

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		source = fragmentShaderSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// 编译失败两个一起删除
			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);

			CORE_ERROR("{0}", infoLog.data());
			CORE_ASSERT(false, "Fragment shader compilation failure!");
			return;
		}

		// 创建program
		m_RendererID = glCreateProgram();
		GLuint program = m_RendererID;

		// 连接shader到program
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);

		// 查询链接情况
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			CORE_ERROR("{0}", infoLog.data());
			CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		// 已经有program了可以删除shader了
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
} // namespace Hoshino