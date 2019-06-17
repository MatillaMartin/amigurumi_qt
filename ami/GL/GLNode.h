#pragma once

#include <glm/vec3.hpp>
#include <glm/matrix.hpp>

namespace ami
{
	class GLNode
	{
	public:
		GLNode(const glm::vec3 & position, const glm::vec3 & lookAt, const glm::vec3 & up);

		void setPosition(const glm::vec3 & position);
		void setLookAt(const glm::vec3 & point, const glm::vec3 & up);

		glm::vec3 position() { return m_position; }
		glm::vec3 lookAt() { return m_lookAt; }

		glm::mat4 model();
		glm::mat4 view();

	private:
		glm::vec3 m_position;
		glm::vec3 m_lookAt;
		glm::vec3 m_up;
	};
}