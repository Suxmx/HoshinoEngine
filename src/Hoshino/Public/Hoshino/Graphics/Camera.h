#pragma once
#include "HoshinoMin.h"
#include <glm/gtc/quaternion.hpp>
namespace Hoshino
{
	class HOSHINO_API Camera
	{
	public:
		Camera() : m_Rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)) {}
		virtual ~Camera() = default;

#pragma region Getter
		inline const glm::mat4& GetProjectionMatrix() const
		{
			return m_ProjectionMatrix;
		}
		inline const glm::mat4& GetViewMatrix() const
		{
			return m_ViewMatrix;
		}
		inline const glm::mat4& GetViewProjectionMatrix() const
		{
			return m_ViewProjectionMatrix;
		}

		inline const glm::quat& GetRotation() const
		{
			return m_Rotation;
		}

#pragma endregion Getter

#pragma region Setter
		// 四元数方法
		inline void SetRotation(const glm::quat& rotation)
		{
			m_Rotation = rotation;
			RecalculateVpMatrix();
		}

		// 设置欧拉角旋转（角度制）
		void SetRotationEuler(float pitch, float yaw, float roll)
		{
			pitch = glm::radians(pitch);
			yaw = glm::radians(yaw);
			roll = glm::radians(roll);
			m_Rotation = glm::quat(glm::vec3(pitch, yaw, roll));
			RecalculateVpMatrix();
		}

		// 围绕特定轴旋转
		void Rotate(float angle, const glm::vec3& axis)
		{
			m_Rotation = glm::rotate(m_Rotation, angle, axis);
			RecalculateVpMatrix();
		}
		inline void SetPosition(const glm::vec3& position)
		{
			m_Position = position;
			RecalculateVpMatrix();
		}
		inline void SetPosition(float x, float y, float z)
		{
			m_Position = glm::vec3(x, y, z);
			RecalculateVpMatrix();
		}
#pragma endregion Setter

	protected:
		virtual void RecalculateVpMatrix() = 0;

	protected:
		glm::mat4 m_ProjectionMatrix = glm::mat4(1);
		glm::mat4 m_ViewMatrix = glm::mat4(1);
		glm::mat4 m_ViewProjectionMatrix = glm::mat4(1);
		glm::vec3 m_Position = glm::vec3(0.0f);
		glm::quat m_Rotation; // 四元数表示旋转
	};

	class HOSHINO_API OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float size, float nearClip, float farClip, float aspectRatio = 16.0f / 9.0f) :
		    m_OrthographicSize(size), m_NearClip(nearClip), m_FarClip(farClip), m_AspectRatio(aspectRatio)
		{
			RecalculateVpMatrix();
		}
		void RecalculateVpMatrix() override;
#pragma region Getter
		inline float GetOrthographicSize() const
		{
			return m_OrthographicSize;
		}
		inline float GetNearClip() const
		{
			return m_NearClip;
		}
		inline float GetFarClip() const
		{
			return m_FarClip;
		}
		inline float GetAspectRatio() const
		{
			return m_AspectRatio;
		}
#pragma endregion Getter

#pragma region Setter
		inline void SetOrthographicSize(float size)
		{
			m_OrthographicSize = size;
			RecalculateVpMatrix();
		}
		inline void SetNearClip(float nearClip)
		{
			m_NearClip = nearClip;
			RecalculateVpMatrix();
		}
		inline void SetFarClip(float farClip)
		{
			m_FarClip = farClip;
			RecalculateVpMatrix();
		}
		inline void SetAspectRatio(float aspectRatio)
		{
			m_AspectRatio = aspectRatio;
			RecalculateVpMatrix();
		}
#pragma endregion Setter

	private:
		float m_OrthographicSize = 1.0f;
		float m_NearClip = -1.0f;
		float m_FarClip = 1.0f;
		float m_AspectRatio = 16.0f / 9.0f; // 16:9
	};

} // namespace Hoshino