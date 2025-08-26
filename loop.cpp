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

void loop(int numVertices=3, SDL_Window* window=nullptr, Vertex* vertices=nullptr, VertexBuffer* vertexBuffer=nullptr) {
    glClearColor(0.5f, 0.0f, 1.0f, 1.0f);//setzt die clear farbe
    glClear(GL_COLOR_BUFFER_BIT);//cleart in der gesetzen farbe
    vertexBuffer->bind();//zum zeichnen bindet es den vao
    vertexBuffer->bindVbo();//zum neu beschreiben des buffers
    glDrawArrays(GL_TRIANGLE_STRIP, 0, numVertices);//zeichnet das dreieck, indem es die punkte des gebindeden vertex buffers benutzt
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), vertices, GL_STATIC_DRAW);
    SDL_GL_SwapWindow(window);//switcht die buffer
    vertexBuffer->unbindVbo();
    vertexBuffer->unbind();
}