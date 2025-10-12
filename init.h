#pragma once
#include <GL/glew.h>

using namespace std;

void openGL_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
void init();