#include "control.h"
#include <iostream>
#include <SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "defines.h"
#include "camera.h"
#include "font.h"
#include "character.h"
#include "weapon.h"

Control::Control() {
    wBool = false;
    sBool = false;
    aBool = false;
    dBool = false;
    eBool = false;
    cBool = false;
    rBool = false;
    shieldBool = false;
    jumpOnProgress = false;
    up = true;
    pause = false;
    blockFunction = false;
    prevTimePause = pauseTime - 2 * pauseTime;
    prevTimeShoot = pistolShootTimeCharacter - 2 * pistolShootTimeCharacter;
    prevTimeReload = reloadTime - 2 * reloadTime;
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
        if(event->key.keysym.sym == SDLK_c) {
            cBool = true;
        }
        if(event->key.keysym.sym == SDLK_r) {
            rBool = true;
            blockFunction = true;
        }
#ifndef Release
        if(event->key.keysym.sym == SDLK_b) {
            clog << "breakpoint" << endl;
        }
#endif
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
        if(event->key.keysym.sym == SDLK_c) {
            cBool = false;
        }
    }
}

void Control::control(Camera* camera, Character* player, float delta, const unsigned int* level, SDL_Event* event, glm::mat4* projection, float gameTime, Font* font, Shader* fontShader, float windowWidth, float windowHeight, SDL_Window* window, Shader* shader, Weapon* weapon) {
    float rightBorder;
    float leftBorder;
    switch(*level) {
        case 1:
            rightBorder = borderRightLevel1;
            leftBorder = borderLeftLevel1;
            break;
        case 2:
            rightBorder = borderRightLevel2;
            leftBorder = borderLeftLevel2;
            break;
        default:
            cout << "level not found" << endl;
            rightBorder = 0.0f;
            leftBorder = 0.0f;
            break;
    }
    glm::vec3 characterPosition = glm::vec3((player->characterModel.model)[3]);
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
        player->characterModel.model = glm::mat4(1.0f);
        player->characterModel.model = glm::translate(player->characterModel.model, glm::vec3(camera->getPosition().x, characterPosition.y, characterPosition.z));
        player->characterModel.model = glm::rotate(player->characterModel.model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        player->characterModel.model = glm::scale(player->characterModel.model, glm::vec3(characterScale));
    }
    else if(!aBool) {
        player->characterModel.model = glm::mat4(1.0f);
        player->characterModel.model = glm::translate(player->characterModel.model, glm::vec3(camera->getPosition().x, characterPosition.y, characterPosition.z));
        player->characterModel.model = glm::scale(player->characterModel.model, glm::vec3(characterScale));
    }
    if(dBool && camera->getPosition().x < rightBorder && !shieldBool) {
        camera->translate(glm::vec3(walkSpeed * delta, 0.0f, 0.0f));
        player->characterModel.model = glm::mat4(1.0f);
        player->characterModel.model = glm::translate(player->characterModel.model, glm::vec3(camera->getPosition().x, characterPosition.y, characterPosition.z));
        player->characterModel.model = glm::rotate(player->characterModel.model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        player->characterModel.model = glm::scale(player->characterModel.model, glm::vec3(characterScale));
    }
    else if(!dBool && !aBool) {
        player->characterModel.model = glm::mat4(1.0f);
        player->characterModel.model = glm::translate(player->characterModel.model, glm::vec3(camera->getPosition().x, characterPosition.y, characterPosition.z));
        player->characterModel.model = glm::scale(player->characterModel.model, glm::vec3(characterScale));
    }
    if(shieldBool && aBool) {//schild links
        player->characterModel.model = glm::mat4(1.0f);
        player->characterModel.model = glm::translate(player->characterModel.model, glm::vec3(camera->getPosition().x, characterPosition.y, characterPosition.z));
        player->characterModel.model = glm::rotate(player->characterModel.model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        player->characterModel.model = glm::scale(player->characterModel.model, glm::vec3(characterScale));
        cout << "schild links" << endl;
    }
    if(shieldBool && dBool && !aBool) {//schild rechts
        player->characterModel.model = glm::mat4(1.0f);
        player->characterModel.model = glm::translate(player->characterModel.model, glm::vec3(camera->getPosition().x, characterPosition.y, characterPosition.z));
        player->characterModel.model = glm::rotate(player->characterModel.model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        player->characterModel.model = glm::scale(player->characterModel.model, glm::vec3(characterScale));
        cout << "schild rechts" << endl;
    }
    if(cBool && dBool && gameTime > prevTimeShoot + pistolShootTimeCharacter && weapon->magazine > 0) {
        weapon->magazine -= 1;
        player->shoot(true, shader, camera);
        prevTimeShoot = gameTime;
    }
    else if(cBool && aBool && gameTime > prevTimeShoot + pistolShootTimeCharacter && weapon->magazine > 0) {
        weapon->magazine -= 1;
        player->shoot(false, shader, camera);
        prevTimeShoot = gameTime;
    }
    else if(cBool && !aBool && !dBool && gameTime > prevTimeShoot + pistolShootTimeCharacter && weapon->magazine > 0) {
        weapon->magazine -= 1;
        player->shoot(true, shader, camera);
        prevTimeShoot = gameTime;
    }
    if(rBool) {
        if(blockFunction) {
            prevTimeReload = gameTime;
            blockFunction = false;
        }
        else if(!blockFunction && gameTime < prevTimeReload + reloadTime) {}//nachlade animation
        else if(!blockFunction && gameTime > prevTimeReload + reloadTime) {
            weapon->magazine = weapon->maxMagazine;
            rBool = false;
        }
    }
    if(jumpOnProgress) {
        characterPosition = glm::vec3((player->characterModel.model)[3]);
        if(up) {
            if(characterPosition.y < jumpHeight) {
                player->characterModel.model = glm::translate(player->characterModel.model, glm::vec3(0.0f, jumpSpeed * delta, 0.0f));
            }
            else {
                up = false;
            }
        }
        else {
            if(characterPosition.y > ground) {
                player->characterModel.model = glm::translate(player->characterModel.model, glm::vec3(0.0f, -jumpSpeed * delta, 0.0f));
            }
            else {
                player->characterModel.model = glm::mat4(1.0f);
                player->characterModel.model = glm::translate(player->characterModel.model, glm::vec3(characterPosition.x, ground, characterPosition.z));
                if(aBool) {
                    player->characterModel.model = glm::rotate(player->characterModel.model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                }
                else if(dBool) {
                    player->characterModel.model = glm::rotate(player->characterModel.model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                }
                player->characterModel.model = glm::scale(player->characterModel.model, glm::vec3(characterScale));
                up = true;
                jumpOnProgress = false;
            }
        }
    }
    if(event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE && gameTime > prevTimePause + pauseTime) {
        wBool = false;
        sBool = false;
        aBool = false;
        dBool = false;
        cBool = false;
        rBool = false;
        shieldBool = false;
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
        prevTimePause = gameTime;
    }
    player->characterModel.modelViewProj = *projection * player->characterModel.model;
}