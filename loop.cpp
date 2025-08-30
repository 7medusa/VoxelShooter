#pragma once

#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>

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

void loop(SDL_Window* window, Vertex* vertices, VertexBuffer* vertexBuffer, int numIndices, Shader* shader, IndexBuffer* indexBuffer) {
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);//setzt die clear farbe
    glClear(GL_COLOR_BUFFER_BIT);//cleart in der gesetzen farbe
    vertexBuffer->bind();//zum zeichnen bindet es den vao
    vertexBuffer->bindVbo();//zum neu beschreiben des buffers
    indexBuffer->bind();
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    SDL_GL_SwapWindow(window);//switcht die buffer
    vertexBuffer->unbindVbo();
    vertexBuffer->unbind();
}