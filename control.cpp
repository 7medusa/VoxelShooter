#include "control.h"
#include <iostream>
#include <SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "defines.h"
#include "camera.h"
#include "font.h"

Control::Control() {
    wBool = false;
    sBool = false;
    aBool = false;
    dBool = false;
    eBool = false;
    shieldBool = false;
    jumpOnProgress = false;
    up = true;
    pause = false;
    prevTime = pauseTime - 2 * pauseTime;
}

void Control::handle(SDL_Event* event, Camera* camera) {
    if(event->type == SDL_KEYDOWN) {
        if(event->key.keysym.sym == SDLK_w) {
            wBool = true;
        }
        if(event->key.keysym.sym == SDLK_s) {
            sBool = true;
        }
        if(event->key.keysym.sym == SDLK_a) {
            aBool = true;
        }
        if(event->key.keysym.sym == SDLK_d) {
            dBool = true;
        }
        if(event->key.keysym.sym == SDLK_f) {
            shieldBool = true;
        }
        if(event->key.keysym.sym == SDLK_e) {
            eBool = true;
        }
        if(event->key.keysym.sym == SDLK_x) {
            camera->reset();
        }
        if(event->key.keysym.sym == SDLK_SPACE && !shieldBool) {
            jumpOnProgress = true;
        }
    }
    if(event->type == SDL_KEYUP) {
        if(event->key.keysym.sym == SDLK_w) {
            wBool = false;
        }
        if(event->key.keysym.sym == SDLK_s) {
            sBool = false;
        }
        if(event->key.keysym.sym == SDLK_a) {
            aBool = false;
        }
        if(event->key.keysym.sym == SDLK_d) {
            dBool = false;
        }
        if(event->key.keysym.sym == SDLK_f) {
            shieldBool = false;
        }
        if(event->key.keysym.sym == SDLK_e) {
            eBool = false;
        }
    }
}

void Control::control(Camera* camera, glm::mat4* characterModel, glm::mat4* characterModelViewProj, float delta, const unsigned int* level, SDL_Event* event, glm::mat4* projectionPointer, float gameTime, Font* font, Shader* fontShader, float windowWidth, float windowHeight, SDL_Window* window) {
    float rightBorder;
    float leftBorder;
    switch(*level) {
        case 1:
            rightBorder = borderRightLevel1;
            leftBorder = borderLeftLevel1;
            break;
        case 2:
            rightBorder = borderRightLevel1;
            leftBorder = borderLeftLevel1;
            break;
        default:
            cout << "level not found" << endl;
            rightBorder = 0.0f;
            leftBorder = 0.0f;
            break;
    }
    glm::vec3 characterPosition = glm::vec3((*characterModel)[3]);
    if(wBool) {
        if(camera->getPosition().z > zoomIn) {
            camera->translate(glm::vec3(0.0f, 0.0f, -zoomSpeed * delta));
        }
    }
    if(sBool) {
        if(camera->getPosition().z < zoomOut) {
            camera->translate(glm::vec3(0.0f, 0.0f, zoomSpeed * delta));
        }
    }
    if(aBool && camera->getPosition().x > leftBorder && !shieldBool) {
        camera->translate(glm::vec3(-walkSpeed * delta, 0.0f, 0.0f));
        *characterModel = glm::mat4(1.0f);
        *characterModel = glm::translate(*characterModel, glm::vec3(camera->getPosition().x, characterPosition.y, characterPosition.z));
        *characterModel = glm::rotate(*characterModel, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        *characterModel = glm::scale(*characterModel, glm::vec3(characterScale));
    }
    else if(!aBool) {
        *characterModel = glm::mat4(1.0f);
        *characterModel = glm::translate(*characterModel, glm::vec3(camera->getPosition().x, characterPosition.y, characterPosition.z));
        *characterModel = glm::scale(*characterModel, glm::vec3(characterScale));
    }
    if(dBool && camera->getPosition().x < rightBorder && !shieldBool) {
        camera->translate(glm::vec3(walkSpeed * delta, 0.0f, 0.0f));
        *characterModel = glm::mat4(1.0f);
        *characterModel = glm::translate(*characterModel, glm::vec3(camera->getPosition().x, characterPosition.y, characterPosition.z));
        *characterModel = glm::rotate(*characterModel, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        *characterModel = glm::scale(*characterModel, glm::vec3(characterScale));
    }
    else if(!dBool && !aBool) {
        *characterModel = glm::mat4(1.0f);
        *characterModel = glm::translate(*characterModel, glm::vec3(camera->getPosition().x, characterPosition.y, characterPosition.z));
        *characterModel = glm::scale(*characterModel, glm::vec3(characterScale));
    }
    if(shieldBool && aBool) {//schild links
        *characterModel = glm::mat4(1.0f);
        *characterModel = glm::translate(*characterModel, glm::vec3(camera->getPosition().x, characterPosition.y, characterPosition.z));
        *characterModel = glm::rotate(*characterModel, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        *characterModel = glm::scale(*characterModel, glm::vec3(characterScale));
        cout << "schild links" << endl;
    }
    if(shieldBool && dBool && !aBool) {//schild rechts
        *characterModel = glm::mat4(1.0f);
        *characterModel = glm::translate(*characterModel, glm::vec3(camera->getPosition().x, characterPosition.y, characterPosition.z));
        *characterModel = glm::rotate(*characterModel, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        *characterModel = glm::scale(*characterModel, glm::vec3(characterScale));
        cout << "schild rechts" << endl;
    }
    if(jumpOnProgress) {
        characterPosition = glm::vec3((*characterModel)[3]);
        if(up) {
            if(characterPosition.y < jumpHeight) {
                *characterModel = glm::translate(*characterModel, glm::vec3(0.0f, jumpSpeed * delta, 0.0f));
            }
            else {
                up = false;
            }
        }
        else {
            if(characterPosition.y > ground) {
                *characterModel = glm::translate(*characterModel, glm::vec3(0.0f, -jumpSpeed * delta, 0.0f));
            }
            else {
                *characterModel = glm::mat4(1.0f);
                *characterModel = glm::translate(*characterModel, glm::vec3(characterPosition.x, ground, characterPosition.z));
                if(aBool) {
                    *characterModel = glm::rotate(*characterModel, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                }
                else if(dBool) {
                    *characterModel = glm::rotate(*characterModel, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                }
                *characterModel = glm::scale(*characterModel, glm::vec3(characterScale));
                up = true;
                jumpOnProgress = false;
            }
        }
    }
    //pause function
    if(event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE && gameTime > prevTime + pauseTime) {
        wBool = false;
        sBool = false;
        aBool = false;
        dBool = false;
        up = false;
        pause = true;
        font->fontDraw(fontShader, window, font, "pause", windowWidth / 2 - font->measureTextWidth("pause", font->cdata) / 2, windowHeight / 2 - windowHeight / 7);
        SDL_GL_SwapWindow(window);
    }
    while(pause) {
        if(SDL_PollEvent(event)) {
            if(event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE) {
                pause = false;
            }
        }
        prevTime = gameTime;
    }
    *characterModelViewProj = *projectionPointer * *characterModel;
}