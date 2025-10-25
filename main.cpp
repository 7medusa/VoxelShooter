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
#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"
#undef STB_IMAGE_IMPLEMENTATION
#include <fstream>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <GL/gl.h>
#include <iostream>

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
#include "init.h"
#include "time.h"
#include "error.h"

int main(int argc, char** argv) {
    init();
    SDL_Window* window;

#ifdef Release
    static int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP;
    unsigned int windowWidth = 2560;
    unsigned int windowHeight = 1440;
#else
    static int flags = SDL_WINDOW_OPENGL;
    unsigned int windowWidth = 800.0f;
    unsigned int windowHeight = 600.0f;
#endif

    window = SDL_CreateWindow("VoxelShooter 0.1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, flags);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if(glewInit()) {
        Error::initError();
    }

    SDL_Event event;
    Time time;
    unsigned int levelWorld = 1;
    bool close = false;
    Font font(fontDir, 80.0f);
    Shader fontShader(vertexShaderFontDir, fragmentShaderFontDir);
    Shader shader(vertexShaderDir, fragmentShaderDir);
    Shader::unbind();
    Camera camera(cameraFov, windowWidth, windowHeight);
    glm::mat4 projection = camera.getViewProjection();
    Character player(&shader, &camera);
    Weapon weaponPlayer;
    Control control;

    const int invModelViewLocation = glGetUniformLocation(shader.getShaderId(), "u_invModelView");
    const int modelViewLocation = glGetUniformLocation(shader.getShaderId(), "u_modelView");
    const int modelViewProjLocation = glGetUniformLocation(shader.getShaderId(), "u_in_model_view_proj");
    if(invModelViewLocation == -1 || modelViewLocation == -1 || modelViewProjLocation == -1) {
        Error::uniformError();
    }

    while(!close) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                close = true;
                return 0;
            }
            control.handle(&event, &camera);
        }

        float color = 1.0f;
        glClearColor(color, color, color, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        time.update();

        control.control(&camera, &player, time.delta, &levelWorld, &event, &projection, time.time, &font, &fontShader, windowWidth, windowHeight, window, &shader, &weaponPlayer);
        camera.update();
        projection = camera.getViewProjection();

        shader.bind();

        setVariables(player.characterModel.modelViewProj, projection, player.characterModel.model, modelViewProjLocation, &player.characterModel.vertexBuffer, &player.characterModel.indexBuffer,
            modelViewLocation, invModelViewLocation, player.characterModel.modelView, player.characterModel.invModelView, &camera);
        player.characterMesh.render();

        switch(levelWorld) {
            case 1:
                if (!level) {
                    font.loading(&fontShader, window, font, windowWidth, windowHeight, "loading data...");
                    level = make_unique<Level>(&camera, &shader, &player.characterModel.model, time.returnDelta(), time.returnTime(), (char*)level1ModelDir, &levelWorld);
                }
                level->logic(projection, modelViewProjLocation, modelViewLocation, invModelViewLocation, &camera, &font, &fontShader, window, &levelWorld, windowWidth, windowHeight, &control, time, &player);
                break;
            case 2:
                level.reset();
                font.loading(&fontShader, window, font, windowWidth, windowHeight, "loading data...");
                level = make_unique<Level>(&camera, &shader, &player.characterModel.model, time.returnDelta(), time.returnTime(), (char*)level2ModelDir, &levelWorld);
                level->logic(projection, modelViewProjLocation, modelViewLocation, invModelViewLocation, &camera, &font, &fontShader, window, &levelWorld, windowWidth, windowHeight, &control, time, &player);
                break;
            default:
                Error::runtimeError();
                assert(false);
        }

        Shader::unbind();

        font.fontDraw(&fontShader, window, &font, to_string(time.fps), 50, 100);
        font.fontDraw(&fontShader, window, &font, to_string(weaponPlayer.magazine), 50, 200);
#ifndef Release
        font.fontDraw(&fontShader, window, &font, to_string(player.characterModel.model[3].x), 300, 300);
#endif

        SDL_GL_SwapWindow(window);
        time.end();
    }
}