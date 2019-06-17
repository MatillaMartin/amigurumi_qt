#pragma once

#include "ami/GL/GLNode.h"

#include <glm/vec3.hpp>
#include <glm/matrix.hpp>

namespace ami
{
	class GLCamera
	{
	public:
		GLCamera() {};
		GLCamera(const glm::vec3 & position,
			const glm::vec3 & lookAt,
			const glm::vec3 & up,
			float fov, float nearClip, float farClip);
		~GLCamera() {};

		/// Sets the vertical field of view
		void setFov(float degrees);
		
		float fov() { return m_fov; }

		GLNode & node() { return m_node; };
		glm::mat4 projection(float aspect = 1.0f);

	private:
		GLNode m_node = GLNode(glm::vec3(0,0,0), glm::vec3(0,0,-1), glm::vec3(0,1,0));

		/// Vertical field of view
		float m_fov = 70.0f;
		float m_near = 0.1f;
		float m_far = 1000.0f;
	};
}