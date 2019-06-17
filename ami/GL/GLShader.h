#pragma once

#include <QFileInfo>
#include <QOpenGLFunctions>

namespace ami
{
	class GLShader : protected QOpenGLFunctions
	{
	public:
		GLShader();

		bool load(const QFileInfo & vertexFile, const QFileInfo & fragmentFile);

		bool isLoaded() { return m_programId != 0; };
		GLuint id() { return m_programId; }

	private:
		GLuint m_programId = 0;
		GLuint m_vertexId = 0;
		GLuint m_fragmentId = 0;
	};
}