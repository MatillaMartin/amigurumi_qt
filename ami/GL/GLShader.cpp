#pragma once

#include "ami/GL/GLShader.h"

#include <QTextStream>
#include <QDebug>

namespace ami
{
	std::string fileToString(const QFileInfo & fileInfo)
	{
		QFile f(fileInfo.path());
		if (!f.open(QFile::ReadOnly | QFile::Text)) return "";
		QTextStream in(&f);
		return in.readAll().toStdString();
	}

	GLShader::GLShader()
	{
	}

	bool GLShader::load(const QFileInfo & vertexFile, const QFileInfo & fragmentFile)
	{
		if (!vertexFile.exists() || !fragmentFile.exists())
		{
			return false;
		}

		GLint success;
		// convert stream into string
		std::string vertexCode= fileToString(vertexFile);
		std::string fragmentCode= fileToString(fragmentFile);

		const char* vertexCodeCStr = vertexCode.c_str();
		const char* fragmentCodeCStr = fragmentCode.c_str();
		// Vertex shader
		m_vertexId = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(m_vertexId, 1, &vertexCodeCStr, NULL);
		glCompileShader(m_vertexId);
		// print compile errors if any
		glGetShaderiv(m_vertexId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char errorLog[512];
			glGetShaderInfoLog(m_vertexId, 512, NULL, errorLog);
			qDebug() << "Vertex Shader compilation failed:\n" << errorLog;
			glDeleteShader(m_vertexId);
			m_vertexId = 0;
			return false;
		};

		// Fragment Shader
		m_fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(m_fragmentId, 1, &fragmentCodeCStr, NULL);
		glCompileShader(m_fragmentId);
		// print compile errors if any
		glGetShaderiv(m_fragmentId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char errorLog[512];
			glGetShaderInfoLog(m_fragmentId, 512, NULL, errorLog);
			qDebug() << "Fragment Shader compilation failed:\n" << errorLog;
			glDeleteShader(m_vertexId);
			m_vertexId = 0;
			glDeleteShader(m_fragmentId);
			m_fragmentId = 0;
			return false;
		};

		m_programId = glCreateProgram();
		glAttachShader(m_programId, m_vertexId);
		glAttachShader(m_programId, m_fragmentId);
		glLinkProgram(m_programId);
		// print linking errors if any
		glGetProgramiv(m_programId, GL_LINK_STATUS, &success);
		if (!success)
		{
			char errorLog[512];
			glGetProgramInfoLog(m_programId, 512, NULL, errorLog);
			qDebug() << "Shader linking failed:\n" << errorLog;
			glDeleteShader(m_vertexId);
			m_vertexId = 0;
			glDeleteShader(m_fragmentId);
			m_fragmentId = 0;
			glDeleteProgram(m_programId);
			m_programId = 0;
			return false;
		}

		glDeleteShader(m_vertexId);
		glDeleteShader(m_fragmentId);

		return true;
	}
}