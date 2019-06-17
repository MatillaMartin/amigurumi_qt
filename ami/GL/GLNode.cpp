#include "GLNode.h"

#include <glm/gtc/matrix_transform.hpp>

namespace ami
{
	GLNode::GLNode(const glm::vec3 & position, const glm::vec3 & lookAt, const glm::vec3 & up)
		:
		m_position(position),
		m_lookAt(lookAt),
		m_up(up)
	{}

	void GLNode::setPosition(const glm::vec3 & position)
	{
		m_position = position;
	}

	void GLNode::setLookAt(const glm::vec3 & point, const glm::vec3 & up)
	{
		m_lookAt = point;
		m_up = glm::normalize(up);
	}

	glm::mat4 GLNode::model()
	{
		return glm::inverse(view());
	}

	glm::mat4 GLNode::view()
	{
		return glm::lookAt(m_position, m_lookAt, m_up);
	}
}
