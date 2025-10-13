#include "init.h"
#ifdef _WIN32
#include <SDL.h>
#pragma comment( lib, "SDL2.lib" )
#pragma comment( lib, "glew32s.lib" )
#pragma comment( lib, "opengl32.lib" )
#else
#include <SDL2/SDL.h>
#endif
#include <iostream>
#include "defines.h"
#include "error.h"

void openGL_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam=nullptr) {
    cout << "opengl debug message: " << message << endl;
}

void init() {
    if(SDL_Init(SDL_INIT_EVERYTHING)) {
        Error::initError();
    }
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
#ifdef Release
    SDL_GL_SetSwapInterval(1);//vsync
    SDL_ShowCursor(SDL_DISABLE);//versteckt den cursor
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);//debug modus
#endif
#ifdef Debug
    glEnable(GL_DEBUG_OUTPUT);//aktiviert debug output
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);//aktiviert die sofortige benachrichtigung
    glDebugMessageCallback(openGL_debug_callback, nullptr);//legt callback fest
#endif
    GLCALL(glEnable(GL_CULL_FACE));//lässt nicht sichtbare dreiecke nicht zeichnen
    GLCALL(glEnable(GL_DEPTH_TEST));//lässt nur die korrekten vertices laden und jene dich nicht zu sehen sind nicht
}