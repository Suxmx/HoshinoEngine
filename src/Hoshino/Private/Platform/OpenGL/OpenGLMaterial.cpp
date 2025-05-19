#include "Platform/OpenGL/OpenGLMaterial.h"
namespace Hoshino
{
    void OpenGLMaterial::Set(const std::string& name, float value)
    {
        m_FloatValues[name] = value;
    }

    void OpenGLMaterial::Set(const std::string& name, int value)
    {
        m_IntValues[name] = value;
    }

    void OpenGLMaterial::Set(const std::string& name, uint32_t value)
    {
        // 存储为int
        m_IntValues[name] = static_cast<int>(value);
    }

    void OpenGLMaterial::Set(const std::string& name, bool value)
    {
        m_BoolValues[name] = value;
    }

    void OpenGLMaterial::Set(const std::string& name, const glm::vec2& value)
    {
        m_Vec2Values[name] = value;
    }

    void OpenGLMaterial::Set(const std::string& name, const glm::vec3& value)
    {
        m_Vec3Values[name] = value;
    }

    void OpenGLMaterial::Set(const std::string& name, const glm::vec4& value)
    {
        m_Vec4Values[name] = value;
    }

    void OpenGLMaterial::Set(const std::string& name, const glm::mat3& value)
    {
        m_Mat3Values[name] = value;
    }

    void OpenGLMaterial::Set(const std::string& name, const glm::mat4& value)
    {
        m_Mat4Values[name] = value;
    }

    void OpenGLMaterial::Apply()
    {
        m_Shader->Bind();

        // 上传所有存储的uniform值
        for (const auto& [name, value] : m_FloatValues)
            m_Shader->UploadUniformFloat(name, value);

        for (const auto& [name, value] : m_IntValues)
            m_Shader->UploadUniformInt(name, value);

        for (const auto& [name, value] : m_BoolValues)
            m_Shader->UploadUniformInt(name, value ? 1 : 0);

        for (const auto& [name, value] : m_Vec2Values)
            m_Shader->UploadUniformFloat2(name, value);

        for (const auto& [name, value] : m_Vec3Values)
            m_Shader->UploadUniformFloat3(name, value);

        for (const auto& [name, value] : m_Vec4Values)
            m_Shader->UploadUniformFloat4(name, value);

        for (const auto& [name, value] : m_Mat3Values)
            m_Shader->UploadUniformMat3(name, value);

        for (const auto& [name, value] : m_Mat4Values)
            m_Shader->UploadUniformMat4(name, value);
    }
} // namespace Hoshino