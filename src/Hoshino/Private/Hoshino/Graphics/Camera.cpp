#include "HoshinoPch.h"
#include "Hoshino/Graphics/Camera.h"

namespace Hoshino
{
	void OrthographicCamera::RecalculateVpMatrix()
	{
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Rotation * glm::vec3(0, 0, -1),
		                           m_Rotation * glm::vec3(0, 1, 0));
		m_ProjectionMatrix =
		    glm::ortho(m_OrthographicSize * m_AspectRatio * -0.5f, m_OrthographicSize * m_AspectRatio * 0.5f,
		               m_OrthographicSize * -0.5f, m_OrthographicSize * 0.5f, m_NearClip, m_FarClip);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
} // namespace Hoshino
