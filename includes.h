#pragma once

#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>

// Platform: SDL include
#ifdef _WIN32
#include <SDL.h>
#pragma comment( lib, "SDL2.lib" )
#pragma comment( lib, "glew32s.lib" )
#pragma comment( lib, "opengl32.lib" )
#else
#include <SDL2/SDL.h>
#endif

#include "defines.h"
#include "vertex_buffer.h"
#include "shader.h"
#include "index_buffer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <GL/gl.h>