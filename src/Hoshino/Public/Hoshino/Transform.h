#pragma once
#include "HoshinoMin.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"

namespace Hoshino
{
	class HOSHINO_API Transform
	{
	public:
		Transform() = default;
		Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) :
		    m_Position(position), m_Rotation(rotation), m_Scale(scale)
		{}

		const glm::mat4& GetTransformMatrix() const
		{
			return m_TransformMatrix;
		}

		inline const glm::vec3& GetPosition() const
		{
			return m_Position;
		}
		inline const glm::vec3& GetRotation() const
		{
			return m_Rotation;
		}
		inline const glm::vec3& GetScale() const
		{
			return m_Scale;
		}

		inline void SetPosition(const glm::vec3& position)
		{
			m_Position = position;
			RecalculateTransformMatrix();
		}
		inline void SetRotation(const glm::vec3& rotation)
		{
			m_Rotation = rotation;
			RecalculateTransformMatrix();
		}
		inline void SetScale(const glm::vec3& scale)
		{
			m_Scale = scale;
			RecalculateTransformMatrix();
		}
		void RecalculateTransformMatrix()
		{
			glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_Position);
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.x), {1.0f, 0.0f, 0.0f});
			rotation = glm::rotate(rotation, glm::radians(m_Rotation.y), {0.0f, 1.0f, 0.0f});
			rotation = glm::rotate(rotation, glm::radians(m_Rotation.z), {0.0f, 0.0f, 1.0f});
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_Scale);

			m_TransformMatrix = translation * rotation * scale;
		}

	private:
		glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
		glm::vec3 m_Rotation = {0.0f, 0.0f, 0.0f};
		glm::vec3 m_Scale = {1.0f, 1.0f, 1.0f};
		glm::mat4 m_TransformMatrix = glm::mat4(1.0f);
	};
} // namespace Hoshino