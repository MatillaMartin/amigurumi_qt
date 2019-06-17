#pragma once
#include "ami/GL/GLCommon.h"
#include <QOpenGLFunctions>
#include <QDebug>
#include <string>

void checkGLError() {
	QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
	GLenum err(glFuncs.glGetError());
	bool bError = false;
	while (err != GL_NO_ERROR) {
		std::string error;

		switch (err) {
		case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;
		case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;
		case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;
		case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
		}

		qDebug() << "GL_" << error.c_str() << " - " << __FILE__ << ":" << __LINE__;
		err = glFuncs.glGetError();
		bError = true;
	}
	
	assert(bError);
}