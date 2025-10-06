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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <GL/gl.h>
#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"
#undef STB_IMAGE_IMPLEMENTATION
#include <fstream>
#include <bits/locale_facets_nonio.h>
#include <vector>

#include "defines.h"
#include "shader.h"
#include "camera.h"
#include "dir.h"
#include "control.h"
#include "mesh.h"
#include "setVariables.h"
#include "model.h"
#include "font.h"

#include "level.h"

void openGL_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam=nullptr) {
#ifdef Debug
    cout << "opengl debug message: " << message << endl;
#endif
}

int main(int argc, char** argv) {
    //initialisiert eine schnittstelle zwischen window manager sdl und opengl
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window;

    //buffergrößen
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    //fenster flag für eigenschaften wie fullscreen
#ifdef Release
    static int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP;
    SDL_GL_SetSwapInterval(1);//vsync
    SDL_ShowCursor(SDL_DISABLE);//versteckt den cursor
    unsigned int windowWidth = 2560;
    unsigned int windowHeight = 1440;
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);//debug modus
    static int flags = SDL_WINDOW_OPENGL;
    unsigned int windowWidth = 800.0f;
    unsigned int windowHeight = 600.0f;
#endif

    //erstellt und definiert eigenschaften für ein fenster
    window = SDL_CreateWindow("VoxelShooter 0.1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, flags);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);//setzt ein kontext damit opengl mit dem window manager sdl kommunizieren kann
    glewInit();

#ifndef Release
    glEnable(GL_DEBUG_OUTPUT);//aktiviert debug output
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);//aktiviert die sofortige benachrichtigung
    glDebugMessageCallback(openGL_debug_callback, nullptr);//legt callback fest
#endif

    //allgemeines
    SDL_Event event;
    unsigned int levelWorld = 1;
    bool close = false;
    Control control;
    Font font(fontDir, 80.0f);
    GLCALL(glEnable(GL_CULL_FACE));//lässt nicht sichtbare dreiecke nicht zeichnen
    GLCALL(glEnable(GL_DEPTH_TEST));//lässt nur die korrekten vertices laden und jene dich nicht zu sehen sind nicht
    Shader fontShader(vertexShaderFontDir, fragmentShaderFontDir);
    Shader shader(vertexShaderDir, fragmentShaderDir);
    Shader::unbind();

    //kamera
    Camera camera(cameraFov, windowWidth, windowHeight);
    camera.translate(glm::vec3(0.0f, 0.0f, 5.0f));
    camera.update();

    //holt sich variablen aus dem shader um deren speicherort zu speichern um die daten darin zu ändern
    const int invModelViewLocation = glGetUniformLocation(shader.getShaderId(), "u_invModelView");
    const int modelViewLocation = glGetUniformLocation(shader.getShaderId(), "u_modelView");
    const int modelViewProjLocation = glGetUniformLocation(shader.getShaderId(), "u_in_model_view_proj");

    //modele
    glm::mat4 projection = camera.getViewProjection();
    Model character(&camera, 0.0f, glm::vec3(0.0f, ground, 0.0f), glm::vec3(characterScale));
    ModelRead characterMesh(characterModelDir, &shader);

    //level
    font.loading(&fontShader, window, font, windowWidth, windowHeight, "loading data...");
    Level1 level1(&camera, &shader, &character.model);
    Level2 level2(&camera, &shader, &character.model);

    const double perfCounterFrequency = static_cast<double>(SDL_GetPerformanceFrequency());
    double lastCounter = static_cast<double>(SDL_GetPerformanceCounter());
    float delta = 0.0f;
    float time = 0.0f;
    int fps = 0;

    while(!close) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                close = true;
                return 0;
            }
            control.handle(&event, &camera);
        }

        camera.update();
        projection = camera.getViewProjection();

        float farbe = 0.0f;
        glClearColor(farbe, farbe, farbe, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        time += delta;

        control.control(&camera, &character.model, &character.modelViewProj, delta, &levelWorld, &event, &projection, time, &font, &fontShader, windowWidth, windowHeight, window);

        shader.bind();
        setVariables(character.modelViewProj, projection, character.model, modelViewProjLocation, &character.vertexBuffer, &character.indexBuffer, modelViewLocation, invModelViewLocation, character.modelView, character.invModelView, &camera);
        characterMesh.render();
        switch(levelWorld) {
            case 1:
                level1.logic(projection, modelViewProjLocation, modelViewLocation, invModelViewLocation, &camera, &font, &fontShader, window, &levelWorld, windowWidth, windowHeight, &control, &event);
                break;
            case 2:
                level2.logic(projection, modelViewProjLocation, modelViewLocation, invModelViewLocation, &camera, &font, &fontShader, window, &levelWorld, windowWidth, windowHeight, &control, &event);
                break;
            default:
                cout << "level not found" << endl;
                break;
        }
        Shader::unbind();

        font.fontDraw(&fontShader, window, &font, to_string(fps), 100, 100);
#ifndef Release
        font.fontDraw(&fontShader, window, &font, to_string(character.model[3].x), 200, 200);
#endif

        SDL_GL_SwapWindow(window);//switcht die buffer

        GLCALL(glBindTexture(GL_TEXTURE_2D, 0));

        //fps
        const double endCounter = static_cast<double>(SDL_GetPerformanceCounter());
        const double counterElapsed = endCounter - lastCounter;
        delta = static_cast<float>(counterElapsed) / static_cast<float>(perfCounterFrequency);
        lastCounter = endCounter;
        fps = 1.0f / delta;
    }
}