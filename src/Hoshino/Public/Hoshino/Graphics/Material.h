#pragma once

#include "HoshinoMin.h"
#include "Hoshino/Graphics/Shader.h"
#include "Hoshino/Graphics/Renderer.h"

namespace Hoshino
{
    class HOSHINO_API Material
    {
    public:
        Material()
        {
            m_Shader = Renderer::GetDefaultShader();
        }
        Material(const Ref<Shader>& shader) : m_Shader(shader) {}
        virtual ~Material() = default;

        virtual void Set(const std::string& name, float value) = 0;
        virtual void Set(const std::string& name, int value) = 0;
        virtual void Set(const std::string& name, uint32_t value) = 0;
        virtual void Set(const std::string& name, bool value) = 0;
        virtual void Set(const std::string& name, const glm::vec2& value) = 0;
        virtual void Set(const std::string& name, const glm::vec3& value) = 0;
        virtual void Set(const std::string& name, const glm::vec4& value) = 0;
        virtual void Set(const std::string& name, const glm::mat3& value) = 0;
        virtual void Set(const std::string& name, const glm::mat4& value) = 0;

        // 新增统一上传函数
        virtual void Apply() = 0;

    public:
        static Ref<Material> Create(const Ref<Shader>& shader);
        static Ref<Material> Create();

    protected:
        Ref<Shader> m_Shader;
    };
} // namespace Hoshino