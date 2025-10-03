#pragma once
#include "includes.h"

class Control {
public:
    bool wBool = false;
    bool sBool = false;
    bool aBool = false;
    bool dBool = false;
    bool shieldBool = false;
    bool jumpOnProgress = false;
    bool up = true;

    void handle(SDL_Event* event, Camera* camera) {
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
        }
    }
    void control(Camera* camera, glm::mat4* characterModel, glm::mat4 projection, glm::mat4* characterModelViewProj, float delta, unsigned int* level, SDL_Event* event, Shader* shader, int modelViewProjLocation, int modelViewLocation, int invModelViewLocation, glm::mat4* projectionPointer) {
        float rightBorder;
        float leftBorder;
        switch(*level) {
            case 1:
                rightBorder = borderRightLevel1;
                leftBorder = borderLeftLevel1;
                break;
            default:
                cout << "level not found" << endl;
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
            *characterModelViewProj = *projectionPointer * *characterModel;
        }
    }
};
