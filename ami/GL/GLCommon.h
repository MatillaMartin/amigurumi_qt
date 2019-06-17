#pragma once

#define GL_CHECK(x) x; checkGLError();

void checkGLError();