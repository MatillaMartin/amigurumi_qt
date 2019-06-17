#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include "ami/GL/GLCamera.h"
#include "ami/GL/GLMesh.h"
#include "ami/GL/GLShader.h"

class QOpenGLShaderProgram;

namespace ami
{
	class GLView : public QOpenGLWidget, protected QOpenGLFunctions
	{
	public:
		GLView(QWidget * parent = nullptr);
		~GLView();

		virtual void initializeGL();
		virtual void resizeGL(int w, int h);
		virtual void paintGL();

		void setCamera(const GLCamera & camera);
		void setShader(const GLShader & shader);

		GLCamera & camera();
		GLShader & shader();

	private:
		GLCamera m_camera;
		GLShader m_shader;
		std::vector<GLMesh> m_meshes;

		float m_aspect;
	};


}