#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#define SDL_MAIN_HANDLED

#ifdef _WIN32
#include <SDL.h>
#pragma comment( lib, "SDL2.lib" )
#pragma comment( lib, "glew32s.lib" )
#pragma comment( lib, "opengl32.lib" )
#else
#include <SDL2/SDL.h>
#endif

using namespace std;

struct Vertex {
    float x;
    float y;
    float z;
};

int main(int argc, char** argv) {
    SDL_Window* window;
    SDL_Init(SDL_INIT_EVERYTHING);//init sdl
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);//legt buffergröße fest
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    window = SDL_CreateWindow("fenster", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);//setzt die werte für das fenster wie name, position, größe und funktion
    SDL_GLContext glContext = SDL_GL_CreateContext(window);//setzt context für das fenster, damit sdl mit opengl kommunizieren kann

    GLenum glewError = glewInit();//initalisiert glew, für mehr opengl funktionen und legt es in eine variable ab
    if (glewError != GLEW_OK) {
        cout << "GLEW error: " << glewGetErrorString(glewError) << endl;
        return -1;
    }
    cout << "opengl version: " << glGetString(GL_VERSION) << endl;

    Vertex vertices[] = {//struktur das als array verbaut ist, das pro punkt eine struktur annihmt, das 3 variablen hat für die 3 räumlichen koordinaten
        Vertex{-0.5f, -0.5f, 0.0f},
        Vertex{0.0f, 0.5f, 0.0f},
        Vertex{0.5f, -0.5f, 0.0f}
    };
    int numVertices = 3;//array größe, also anzahl an punkten des dreiecks

    GLuint vertexBuffer;//erstellt ein vertex buffer und eine passenden index
    glGenBuffers(1, &vertexBuffer);//generiert den vertex buffer, der die punkte des dreiecks enthält und erzeugt ein index
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);//bindet den vertex buffer an die aktuelle opengl kontext, damit er benutzt werden kann
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), vertices, GL_STATIC_DRAW);//übergibt die punkte des dreiecks an den vertex buffer, damit sie von opengl benutzt werden können
    glEnableVertexAttribArray(0);//aktiviert das vertex mit index 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);//legt fest, wie die punkte des dreiecks im vertex buffer gespeichert sind, damit opengl sie richtig interpretieren kann

    bool close = false;
    while(!close) {
        glClearColor(0.5f, 0.0f, 1.0f, 1.0f);//setzt die clear farbe
        glClear(GL_COLOR_BUFFER_BIT);//cleart in der gesetzen farbe
        //gameloop
        glDrawArrays(GL_TRIANGLES, 0, numVertices);//zeichnet das dreieck, indem es die punkte des vertex buffers benutzt
        //gameloop
        SDL_GL_SwapWindow(window);//switcht die buffer

        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                close = true;
            }
        }
    }
    return 0;
}