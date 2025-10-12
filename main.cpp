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
#include <memory>

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
#include "character.h"
#include "projektil.h"
#include "weapon.h"

void openGL_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam=nullptr) {
    cout << "opengl debug message: " << message << endl;
}

int main(int argc, char** argv) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window;

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

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

#ifdef Debug
    glEnable(GL_DEBUG_OUTPUT);//aktiviert debug output
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);//aktiviert die sofortige benachrichtigung
    glDebugMessageCallback(openGL_debug_callback, nullptr);//legt callback fest
#endif

    unique_ptr<Level1> level1;
    unique_ptr<Level2> level2;
    SDL_Event event;
    unsigned int levelWorld = 1;
    bool close = false;
    GLCALL(glEnable(GL_CULL_FACE));//lässt nicht sichtbare dreiecke nicht zeichnen
    GLCALL(glEnable(GL_DEPTH_TEST));//lässt nur die korrekten vertices laden und jene dich nicht zu sehen sind nicht
    Font font(fontDir, 80.0f);
    Shader fontShader(vertexShaderFontDir, fragmentShaderFontDir);
    Shader shader(vertexShaderDir, fragmentShaderDir);
    Shader::unbind();
    Camera camera(cameraFov, windowWidth, windowHeight);
    camera.translate(glm::vec3(0.0f, 0.0f, 5.0f));
    camera.update();
    glm::mat4 projection = camera.getViewProjection();
    Character player(&shader, &camera);
    Weapon weaponPlayer;//spätere waffenauswahlfunktion
    Control control(weaponPlayer.shootTime);

    const int invModelViewLocation = glGetUniformLocation(shader.getShaderId(), "u_invModelView");
    const int modelViewLocation = glGetUniformLocation(shader.getShaderId(), "u_modelView");
    const int modelViewProjLocation = glGetUniformLocation(shader.getShaderId(), "u_in_model_view_proj");

    const double perfCounterFrequency = static_cast<double>(SDL_GetPerformanceFrequency());
    double lastCounter = static_cast<double>(SDL_GetPerformanceCounter());
    float delta = 0.0f;
    float time = 0.0f;
    int fps;

    while(!close) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                close = true;
                return 0;
            }
            control.handle(&event, &camera);
        }

        float farbe = 0.0f;
        glClearColor(farbe, farbe, farbe, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        time += delta;

        control.control(&camera, &player, delta, &levelWorld, &event, &projection, time, &font, &fontShader, windowWidth, windowHeight, window, &shader, &weaponPlayer);
        camera.update();
        projection = camera.getViewProjection();

        shader.bind();

        setVariables(player.characterModel.modelViewProj, projection, player.characterModel.model, modelViewProjLocation, &player.characterModel.vertexBuffer, &player.characterModel.indexBuffer,
            modelViewLocation, invModelViewLocation, player.characterModel.modelView, player.characterModel.invModelView, &camera);
        player.characterMesh.render();

        iteratorProjektile(&characterProjektile, &camera, projection, modelViewProjLocation, modelViewLocation, invModelViewLocation, delta);//bewegt die patrone
        //iteratorProjektile(enemyProjektile, &camera, projection, modelViewProjLocation, modelViewLocation, invModelViewLocation);

        switch(levelWorld) {
            case 1:
                if (!level1) {
                    font.loading(&fontShader, window, font, windowWidth, windowHeight, "loading data...");
                    level1 = make_unique<Level1>(&camera, &shader, &player.characterModel.model);
                }
                level1->logic(projection, modelViewProjLocation, modelViewLocation, invModelViewLocation, &camera, &font, &fontShader, window, &levelWorld, windowWidth, windowHeight, &control, &event);
                break;
            case 2:
                if (level1) {level1.reset();}
                if (!level2) {
                    font.loading(&fontShader, window, font, windowWidth, windowHeight, "loading data...");
                    level2 = make_unique<Level2>(&camera, &shader, &player.characterModel.model);
                }
                level2->logic(projection, modelViewProjLocation, modelViewLocation, invModelViewLocation, &camera, &font, &fontShader, window, &levelWorld, windowWidth, windowHeight, &control, &event);
                break;
            default:
                level1.reset();
                level2.reset();
                cout << "level not found" << endl;
                break;
        }
        Shader::unbind();

        font.fontDraw(&fontShader, window, &font, to_string(fps), 50, 100);
        font.fontDraw(&fontShader, window, &font, to_string(weaponPlayer.magazine), 50, 200);
#ifndef Release
        font.fontDraw(&fontShader, window, &font, to_string(player.characterModel.model[3].x), 300, 300);
#endif

        SDL_GL_SwapWindow(window);//switcht die buffer

        const double endCounter = static_cast<double>(SDL_GetPerformanceCounter());
        const double counterElapsed = endCounter - lastCounter;
        delta = static_cast<float>(counterElapsed) / static_cast<float>(perfCounterFrequency);
        lastCounter = endCounter;
        fps = 1.0f / delta;
    }
}