#include "GLCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace ami
{
	GLCamera::GLCamera(const glm::vec3 & position, const glm::vec3 & lookAt, const glm::vec3 & up, float fov, float nearClip, float farClip)
		:
		m_node(position, lookAt, up),
		m_fov(fov),
		m_near(nearClip),
		m_far(farClip)
	{}

	void GLCamera::setFov(float degrees)
	{
		m_fov = degrees;
	}
	
	glm::mat4 GLCamera::projection(float aspect)
	{
		return glm::perspective(glm::radians(m_fov), aspect, m_near, m_far);
	}
}
