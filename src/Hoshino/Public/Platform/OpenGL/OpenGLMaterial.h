#pragma once
#include "Hoshino/Graphics/Material.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Hoshino
{
    class HOSHINO_API OpenGLMaterial : public Material
    {
    public:
		OpenGLMaterial(){}
        OpenGLMaterial(const Ref<Shader>& shader) : m_Shader(shader) {}
        virtual ~OpenGLMaterial() = default;

        virtual void Set(const std::string& name, float value) override;
        virtual void Set(const std::string& name, int value) override;
        virtual void Set(const std::string& name, uint32_t value) override;
        virtual void Set(const std::string& name, bool value) override;
        virtual void Set(const std::string& name, const glm::vec2& value) override;
        virtual void Set(const std::string& name, const glm::vec3& value) override;
        virtual void Set(const std::string& name, const glm::vec4& value) override;
        virtual void Set(const std::string& name, const glm::mat3& value) override;
        virtual void Set(const std::string& name, const glm::mat4& value) override;

        // 新增的统一上传函数
        virtual void Apply() override;

    private:
        Ref<Shader> m_Shader;

        // 存储各种类型的uniform值
        std::unordered_map<std::string, float> m_FloatValues;
        std::unordered_map<std::string, int> m_IntValues;
        std::unordered_map<std::string, bool> m_BoolValues;
        std::unordered_map<std::string, glm::vec2> m_Vec2Values;
        std::unordered_map<std::string, glm::vec3> m_Vec3Values;
        std::unordered_map<std::string, glm::vec4> m_Vec4Values;
        std::unordered_map<std::string, glm::mat3> m_Mat3Values;
        std::unordered_map<std::string, glm::mat4> m_Mat4Values;
    };
} // namespace Hoshino