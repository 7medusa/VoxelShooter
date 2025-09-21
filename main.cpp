#include "includes.h"
#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <bits/locale_facets_nonio.h>

#include "control.cpp"
#include "model.cpp"
#include "draw.cpp"
#include "character.cpp"

void openGL_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam=nullptr) {
#ifdef Release
    if(severity == GL_DEBUG_SEVERITY_HIGH || severity == GL_DEBUG_SEVERITY_MEDIUM) {
        cout << "opengl error message: " << message << endl;
    }
#else
    cout << "opengl debug message: " << message << endl;
#endif
}

int main() {
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
    int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP;
    SDL_GL_SetSwapInterval(1);//vsync
    SDL_ShowCursor(SDL_DISABLE);//versteckt den cursor
    int windowWidth = 2560.0f;
    int windowHeight = 1440.0f;
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

    //kamera
    Camera camera(90.0f, windowWidth, windowHeight);
    camera.translate(glm::vec3(0.0f, 0.0f, 5.0f));
    camera.update();

    //modele
    glm::mat4 projection = camera.getViewProjection();

    Model sphere(static_cast<string>(sphereModelDir), 0, 1, 1);
    glm::mat4 sphereModel = glm::mat4(1.0f);//matrix zur positionierung der vertices
    glm::mat4 sphereModelViewProj = projection * sphereModel;//gesamte matrix zur vereinfachung um es an den shader zu übergeben
    VertexBuffer vertexBufferSphere(sphere.vertices.data(), sphere.numVertices);
    IndexBuffer indexBufferSphere(sphere.indices.data(), sphere.numIndices, sizeof(sphere.indices[0]));

    Model character(static_cast<string>(characterModelDir), 1, 0, 0);
    glm::mat4 characterModel = glm::mat4(1.0f);
    glm::mat4 characterModelViewProj = projection * characterModel;
    VertexBuffer vertexBufferCharacter(character.vertices.data(), character.numVertices);
    IndexBuffer indexBufferCharacter(character.indices.data(), character.numIndices, sizeof(character.indices[0]));

    //allgemeines
    bool close = false;
    Control control;
    GLCALL(glEnable(GL_CULL_FACE));//lässt nicht sichtbare dreiecke nicht zeichnen
    GLCALL(glEnable(GL_DEPTH_TEST));//lässt nur die korrekten vertices laden
    const Shader shader(vertexShaderDir, fragmentShaderDir);
    shader.bind();

    //holt sich variablen aus dem shader um deren speicherort zu speichern um die daten darin zu ändern
    const int colorUniformLocation = glGetUniformLocation(shader.getShaderId(), "u_in_color");
    glUniform4f(colorUniformLocation, 0.0f, 0.0f, 0.0f, 1.0f);
    const int modelViewProjLocation = glGetUniformLocation(shader.getShaderId(), "u_in_model_view_proj");
    GLCALL(glUniformMatrix4fv(modelViewProjLocation, 1, GL_FALSE, &sphereModelViewProj[0][0]));
    GLCALL(glUniformMatrix4fv(modelViewProjLocation, 1, GL_FALSE, &characterModelViewProj[0][0]));

    const double perfCounterFrequency = static_cast<double>(SDL_GetPerformanceFrequency());
    double lastCounter = static_cast<double>(SDL_GetPerformanceCounter());
    float delta = 0.0f;
    float time = 0.0f;

    //objekte
    Character player(3);

    while(!close) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                close = true;
                return 0;
            }
            control.handle(&event, &camera, &characterModel, &projection, &characterModelViewProj);
        }

        control.control(&event, &camera, &characterModel, &projection, &characterModelViewProj);
        camera.update();
        projection = camera.getViewProjection();

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//cleart den zu bearbeitenden buffer
        glUniform4f(colorUniformLocation, 0.5f, 0.0f, 1.0f, 1.0f);

        time += delta;

        draw(sphereModelViewProj, projection, sphereModel, modelViewProjLocation, sphere.numIndices, &vertexBufferSphere, &indexBufferSphere);
        player.draw(characterModelViewProj, projection, characterModel, modelViewProjLocation, character.numIndices, &vertexBufferCharacter, &indexBufferCharacter, time, &camera);

        SDL_GL_SwapWindow(window);//switcht die buffer

        GLCALL(glBindTexture(GL_TEXTURE_2D, 0));

        //fps
        const double endCounter = static_cast<double>(SDL_GetPerformanceCounter());
        const double counterElapsed = endCounter - lastCounter;
        delta = static_cast<float>(counterElapsed) / static_cast<float>(perfCounterFrequency);
        lastCounter = endCounter;
    }
}