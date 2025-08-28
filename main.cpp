#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <iterator>
#ifdef _WIN32
#include <SDL.h>
#pragma comment( lib, "SDL2.lib" )
#pragma comment( lib, "glew32s.lib" )
#pragma comment( lib, "opengl32.lib" )
#else
#include <SDL2/SDL.h>
#endif

using namespace std;

#include "defines.h"
#include "vertex_buffer.h"
#include "shader.h"
#include "index_buffer.h"
#include "loop.cpp"

int main(int argc, char** argv) {
    SDL_Window* window;
    SDL_Init(SDL_INIT_EVERYTHING);//init sdl
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);//legt buffergröße fest
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

#ifdef Release
    float flags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP;//legt fenster flag fest wie vollbild oder fullscreen
#else
    float flags = SDL_WINDOW_OPENGL;
#endif

    window = SDL_CreateWindow("fenster", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, flags);//setzt die werte für das fenster wie name, position, größe und funktion
    SDL_GLContext glContext = SDL_GL_CreateContext(window);//setzt context für das fenster, damit sdl mit opengl kommunizieren kann, eine art schnittstelle

    GLenum glewError = glewInit();//initalisiert glew, für mehr opengl funktionen und legt es in eine variable ab
    if (glewError != GLEW_OK) {
        cout << "GLEW error: " << glewGetErrorString(glewError) << endl;
        return -1;
    }
    cout << "opengl version: " << glGetString(GL_VERSION) << endl;

    Vertex vertices[] = {//dreieck koordinaten
        Vertex{-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},
        Vertex{-0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
        Vertex{0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f},
        Vertex{0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},
    };
    int numVertices = sizeof(vertices)/sizeof(vertices[0]);//array größe, also anzahl an punkten der dreiecke

    unsigned int indices[] = {//index für vertices
        0, 1, 2,//dreieck 1
        1, 2, 3//dreieck 2
    };
    int numIndices = sizeof(indices)/sizeof(indices[0]);

    IndexBuffer indexBuffer(indices, numIndices, sizeof(indices[0]));//erstellt ein index buffer aufgrundlage der klasse in index_buffer.h
    VertexBuffer vertexBuffer(vertices, numVertices);//erstellt ein vertex buffer aufgrundlage der klasse in vertex_buffer.h

    Shader shader("/home/medusa/projekte/opengl/shaders/basic.vs", "/home/medusa/projekte/opengl/shaders/basic.fs");//erstellt ein shader objekt, das die vertex und fragment shader lädt und kompiliert

    //fps code
    double perfCounterFrequency = SDL_GetPerformanceFrequency();
    double lastCounter = SDL_GetPerformanceCounter();
    float delta = 0.0f;

    bool close = false;
    while(!close) {
        loop(window, vertices, &vertexBuffer, numIndices, &shader, &indexBuffer);
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                close = true;
            }
        }

        //fps berechnung
        double endCounter = SDL_GetPerformanceCounter();
        double counterElapsed = endCounter - lastCounter;
        delta = ((float)counterElapsed) / (float)perfCounterFrequency;
        float fps = (float)perfCounterFrequency / (float)counterElapsed;
        #ifdef Fps
        cout << "fps: " << (int)fps << endl;
        #endif
        lastCounter = endCounter;
    }
    return 0;
}