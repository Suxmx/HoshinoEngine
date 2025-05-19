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

	void PerspectiveCamera::RecalculateVpMatrix()
	{
	    // 创建视图矩阵 - 与正交相机类似，基于位置和旋转
	    m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Rotation * glm::vec3(0, 0, -1),
	                               m_Rotation * glm::vec3(0, 1, 0));
	    
	    // 创建透视投影矩阵 - 使用垂直FOV、宽高比、近裁剪面和远裁剪面
	    m_ProjectionMatrix = glm::perspective(glm::radians(m_VerticalFOV), m_AspectRatio, m_NearClip, m_FarClip);
	    
	    // 组合视图和投影矩阵
	    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
} // namespace Hoshino
