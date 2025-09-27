#include "includes.h"
#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"
#undef STB_IMAGE_IMPLEMENTATION
#include <vector>
#include <fstream>
#include <iostream>
#include <bits/locale_facets_nonio.h>

#include "control.cpp"
#include "model.cpp"
#include "character.cpp"
#include "mesh.h"

void openGL_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam=nullptr) {
#ifdef Release
    if(severity == GL_DEBUG_SEVERITY_HIGH || severity == GL_DEBUG_SEVERITY_MEDIUM) {
        cout << "opengl error message: " << message << endl;
    }
#else
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
    constexpr int windowWidth = 2560.0f;
    constexpr int windowHeight = 1440.0f;
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);//debug modus
    static int flags = SDL_WINDOW_OPENGL;
    float windowWidth = 800.0f;
    float windowHeight = 600.0f;
#endif

    //erstellt und definiert eigenschaften für ein fenster
    window = SDL_CreateWindow("main", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, flags);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);//setzt ein kontext damit opengl mit dem window manager sdl kommunizieren kann
    glewInit();

#ifndef Release
    glEnable(GL_DEBUG_OUTPUT);//aktiviert debug output
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);//aktiviert die sofortige benachrichtigung
    glDebugMessageCallback(openGL_debug_callback, nullptr);//legt callback fest
#endif

    //allgemeines
    bool close = false;
    Control control;
    GLCALL(glEnable(GL_CULL_FACE));//lässt nicht sichtbare dreiecke nicht zeichnen
    GLCALL(glEnable(GL_DEPTH_TEST));//lässt nur die korrekten vertices laden und jene dich nicht zu sehen sind nicht
    Shader shader(vertexShaderDir, fragmentShaderDir);
    shader.bind();

    //kamera
    Camera camera(90.0f, windowWidth, windowHeight);
    camera.translate(glm::vec3(0.0f, 0.0f, 5.0f));
    camera.update();

    //holt sich variablen aus dem shader um deren speicherort zu speichern um die daten darin zu ändern
    const int invModelViewLocation = glGetUniformLocation(shader.getShaderId(), "u_invModelView");
    const int modelViewLocation = glGetUniformLocation(shader.getShaderId(), "u_modelView");
    const int modelViewProjLocation = glGetUniformLocation(shader.getShaderId(), "u_in_model_view_proj");

    //modele
    glm::mat4 projection = camera.getViewProjection();

    Model debug(debugModelDir, &camera, 90.0f);
    ModelRead debugMesh(debugModelDir, &shader);

    Model character(characterModelDir, &camera, 0.0f, glm::vec3(0.0f, 0.0f, 0.0f));
    ModelRead characterMesh(characterModelDir, &shader);

    //Model level(debugModelDir, &camera, 3.1415926536f, glm::vec3(0.0f, -1.0f, 2.8f));
    //ModelRead levelMesh(debugModelDir, &shader);

    const double perfCounterFrequency = static_cast<double>(SDL_GetPerformanceFrequency());
    double lastCounter = static_cast<double>(SDL_GetPerformanceCounter());
    float delta = 0.0f;
    float time = 0.0f;

    //objekte
    Character player;

    while(!close) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                close = true;
                return 0;
            }
            control.handle(&event, &camera);
        }

        control.control(&event, &camera, &character.model, &projection, &character.modelViewProj, delta);
        camera.update();
        projection = camera.getViewProjection();

        glClearColor(1.0f, 1.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//cleart den zu bearbeitenden buffer

        time += delta;

        //player.setVariables(character.modelViewProj, projection, character.model, modelViewProjLocation, character.numIndices, &character.vertexBuffer, &character.indexBuffer, time, &camera, &shader, character.modelView, character.invModelView, modelViewLocation, invModelViewLocation);
        //characterMesh.render();
        setVariables(debug.modelViewProj, projection, debug.model, modelViewProjLocation, debug.numIndices, &debug.vertexBuffer, &debug.indexBuffer, modelViewLocation, invModelViewLocation, debug.modelView, debug.invModelView, &camera);
        debugMesh.render();
        //setVariables(level.modelViewProj, projection, level.model, modelViewProjLocation, level.numIndices, &level.vertexBuffer, &level.indexBuffer, modelViewLocation, invModelViewLocation, level.modelView, level.invModelView, &camera);
        //levelMesh.render();

        SDL_GL_SwapWindow(window);//switcht die buffer

        GLCALL(glBindTexture(GL_TEXTURE_2D, 0));

        //fps
        const double endCounter = static_cast<double>(SDL_GetPerformanceCounter());
        const double counterElapsed = endCounter - lastCounter;
        delta = static_cast<float>(counterElapsed) / static_cast<float>(perfCounterFrequency);
        lastCounter = endCounter;
    }
}