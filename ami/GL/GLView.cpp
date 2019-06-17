#include "ami/GL/GLView.h"
#include "ami/GL/GLNode.h"
#include "ami/GL/GLCommon.h"

namespace ami
{
	GLView::GLView(QWidget * parent)
		:
		QOpenGLWidget(parent)
	{}

	GLView::~GLView()
	{}

	void GLView::initializeGL()
	{
	}

	void GLView::resizeGL(int w, int h)
	{
		m_aspect = static_cast<double>(w) / (h ? h : 1);
	}

	void GLView::paintGL()
	{
		glm::mat4 view = m_camera.node().view();
		glm::mat4 projection = m_camera.projection(m_aspect);
		for (auto & mesh : m_meshes)
		{
			glm::mat4 model = mesh.node().model();
			glm::mat4 mvp = projection * view * model;

			// load mvp to shader
			GLuint mvpID = glGetUniformLocation(m_shader.id(), "mvp");
			glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);
		}
	}

	void GLView::setCamera(const GLCamera & camera)
	{
		m_camera = camera;
	}

	void GLView::setShader(const GLShader & shader)
	{
		m_shader = shader;
	}
}