#include "includes.h"
#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <bits/locale_facets_nonio.h>

#include "model.cpp"

void openGL_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam=nullptr) {
#ifdef Release
    if(severity == GL_DEBUG_SEVERITY_HIGH || severity == GL_DEBUG_SEVERITY_MEDIUM) {
        cout << "opengl error message: " << message << endl;
    }
#else
    cout << "opengl debug message: " << message << endl;
#endif
}

void unbind() {
    VertexBuffer::unbind();
    IndexBuffer::unbind();
}

void draw(glm::mat4 modelViewProj, glm::mat4 projection, glm::mat4 model, const int modelViewProjLocation, int64_t numIndices, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer) {
    vertexBuffer->bind();
    indexBuffer->bind();
    modelViewProj = projection * model;
    GLCALL(glUniformMatrix4fv(modelViewProjLocation, 1, GL_FALSE, &modelViewProj[0][0]));//ändert die daten in der modelViewPorjLocation im shader
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
    unbind();
}

int main() {
    //initialisiert eine schnittstelle zwischen window manager sdl und opengl
    SDL_Init(SDL_INIT_EVERYTHING);

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
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);//debug modus
    static int flags = SDL_WINDOW_OPENGL;
#endif

    //erstellt und definiert eigenschaften für ein fenster
    SDL_Window* window = SDL_CreateWindow("main", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, flags);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);//setzt ein kontext damit opengl mit dem window manager sdl kommunizieren kann

    //initialisiert glew für mehr funktionen
    glewInit();

    //debug modus
#ifndef Release
    cout << "opengl version: " << glGetString(GL_VERSION) << endl;
    glEnable(GL_DEBUG_OUTPUT);//aktiviert debug output
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);//aktiviert die sofortige benachrichtigung
    glDebugMessageCallback(openGL_debug_callback, nullptr);//legt callback fest
#endif

    //kamera
    Camera camera(90.0f, windowWidth, windowHeight);
    camera.translate(glm::vec3(0.0f, 0.0f, 5.0f));
    camera.update();

    //textur
    int textureWidth;
    int textureHeight;
    int bitsPerPixel;
    stbi_set_flip_vertically_on_load(true);//flippt das koordinatensystem da opengl ein anderes system nutzt
    const auto textureBuffer = stbi_load(textureDir, &textureWidth, &textureHeight, &bitsPerPixel, 4);//lädt die textur
    GLuint textureId;
    GLCALL(glGenTextures(1, &textureId));//erstellt ein texture id mit 1 textur
    GLCALL(glBindTexture(GL_TEXTURE_2D, textureId));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));//filtert die größe der textur
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));//setzt die eigenschaft die am rand der textur passieren soll
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer));//schreibt die textur in opengl
    GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
    if(textureBuffer) {
        stbi_image_free(textureBuffer);//gibt den speicher wieder frei
    }

    //matrizen
    glm::mat4 projection = camera.getViewProjection();

    //koordinaten
    Model sphere(static_cast<string>(sphereModelDir), 0, 0, 1);
    glm::mat4 sphereModel = glm::mat4(1.0f);//matrix zur positionierung der vertices
    glm::mat4 sphereModelViewProj = projection * sphereModel;//gesamte matrix zur vereinfachung um es an den shader zu übergeben
    VertexBuffer vertexBufferSphere(sphere.vertices.data(), sphere.numVertices);
    IndexBuffer indexBufferSphere(sphere.indices.data(), sphere.numIndices, sizeof(sphere.indices[0]));

    Model monkey(static_cast<string>(monkeyModelDir), 0, 1, 0);
    glm::mat4 monkeyModel = glm::mat4(1.0f);
    glm::mat4 monkeyModelViewProj = projection * monkeyModel;
    VertexBuffer vertexBufferMonkey(monkey.vertices.data(), monkey.numVertices);
    IndexBuffer indexBufferMonkey(monkey.indices.data(), monkey.numIndices, sizeof(monkey.indices[0]));

    Model character(static_cast<string>(characterModelDir), 1, 0, 0);
    glm::mat4 characterModel = glm::mat4(1.0f);
    glm::mat4 characterModelViewProj = projection * characterModel;
    const VertexBuffer vertexBufferCharacter(character.vertices.data(), character.numVertices);
    const IndexBuffer indexBufferCharacter(character.indices.data(), character.numIndices, sizeof(character.indices[0]));

    const Shader shader(vertexShaderDir, fragmentShaderDir);
    shader.bind();

    //holt sich variablen aus dem shader um deren speicherort zu speichern um die daten darin zu ändern
    const int colorUniformLocation = glGetUniformLocation(shader.getShaderId(), "u_in_color");
    glUniform4f(colorUniformLocation, 0.0f, 0.0f, 0.0f, 1.0f);
    const int textureUniformLocation = GLCALL(glGetUniformLocation(shader.getShaderId(), "u_in_texture"));
    GLCALL(glUniform1d(textureUniformLocation, 0));
    const int modelViewProjLocation = glGetUniformLocation(shader.getShaderId(), "u_in_model_view_proj");
    GLCALL(glUniformMatrix4fv(modelViewProjLocation, 1, GL_FALSE, &sphereModelViewProj[0][0]));
    GLCALL(glUniformMatrix4fv(modelViewProjLocation, 1, GL_FALSE, &monkeyModelViewProj[0][0]));
    GLCALL(glUniformMatrix4fv(modelViewProjLocation, 1, GL_FALSE, &characterModelViewProj[0][0]));

    const double perfCounterFrequency = static_cast<double>(SDL_GetPerformanceFrequency());
    double lastCounter = static_cast<double>(SDL_GetPerformanceCounter());
    float delta = 0.0f;
    float time = 0.0f;

    bool close = false;

    //tasten
    bool wBool = false;
    bool sBool = false;
    bool aBool = false;
    bool dBool = false;
    bool jumpOnProgress = false;
    bool up = true;

    while(!close) {
        //*loop*//
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                close = true;
            }
            else {
                //tasteneingaben
                if(event.type == SDL_KEYDOWN) {
                    if(event.key.keysym.sym == SDLK_w) {
                        wBool = true;
                    }
                    else if(event.key.keysym.sym == SDLK_s) {
                        sBool = true;
                    }
                    else if(event.key.keysym.sym == SDLK_a) {
                        aBool = true;
                    }
                    else if(event.key.keysym.sym == SDLK_d) {
                        dBool = true;
                    }
                    else if(event.key.keysym.sym == SDLK_SPACE) {
                        jumpOnProgress = true;
                    }
                }
                else if(event.type == SDL_KEYUP) {
                    if(event.key.keysym.sym == SDLK_w) {
                        wBool = false;
                    }
                    else if(event.key.keysym.sym == SDLK_s) {
                        sBool = false;
                    }
                    else if(event.key.keysym.sym == SDLK_a) {
                        aBool = false;
                    }
                    else if(event.key.keysym.sym == SDLK_d) {
                        dBool = false;
                    }
                    else if(event.key.keysym.sym == SDLK_x) {
                        camera.reset();
                    }
                }
            }
        }

        //kamerasteuerung
        if(wBool) {
            if(camera.getPosition().z > 1.7f) {
                camera.translate(glm::vec3(0.0f, 0.0f, -0.04f));
            }
        }
        if(sBool) {
            if(camera.getPosition().z < 13.0f) {
                camera.translate(glm::vec3(0.0f, 0.0f, 0.04f));
            }
        }
        if(aBool) {
            camera.translate(glm::vec3(-0.03f, 0.0f, 0.0f));
        }
        if(dBool) {
            camera.translate(glm::vec3(0.03f, 0.0f, 0.0f));
        }
        if(jumpOnProgress) {
            glm::vec3 characterPosition= glm::vec3(characterModel[3]);
            if(up) {
                if(characterPosition.y < 1.0f) {
                    characterModel = glm::translate(characterModel, glm::vec3(0.0f, 0.06f, 0.0f));
                }
                else {
                    up = false;
                }
            }
            else {
                if(characterPosition.y > 0.0f) {
                    characterModel = glm::translate(characterModel, glm::vec3(0.0f, -0.06f, 0.0f));
                }
                else {
                    characterModel = glm::mat4(1.0f);
                    characterModel = glm::translate(characterModel, glm::vec3(characterPosition.x, 0.0f, characterPosition.x));//reseten der position auf genau 0
                    up = true;
                    jumpOnProgress = false;
                }
            }
            characterModelViewProj = projection * characterModel;
        }

        camera.update();
        projection = camera.getViewProjection();

        glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);//cleart den zu bearbeitenden buffer
        GLCALL(glActiveTexture(GL_TEXTURE0));//aktiviert textur unit 0 um die textur zu binden
        GLCALL(glBindTexture(GL_TEXTURE_2D, textureId));//bindet die textur
        glUniform4f(colorUniformLocation, 0.5f, 0.0f, 1.0f, 1.0f);

        time += delta;

        draw(monkeyModelViewProj, projection, monkeyModel, modelViewProjLocation, monkey.numIndices, &vertexBufferMonkey, &indexBufferMonkey);
        draw(sphereModelViewProj, projection, sphereModel, modelViewProjLocation, sphere.numIndices, &vertexBufferSphere, &indexBufferSphere);

        vertexBufferCharacter.bind();
        indexBufferCharacter.bind();
        float characterYCoordinate = glm::vec3(characterModel[3]).y;//zwischenspeichern für springen funktion
        characterModel = glm::mat4(1.0f);//zurücksetzen der matrix um mit translate nicht zu addieren
        characterModel = glm::translate(characterModel, glm::vec3(camera.getPosition().x, characterYCoordinate, 0.0f));
        characterModelViewProj = projection * characterModel;
        GLCALL(glUniformMatrix4fv(modelViewProjLocation, 1, GL_FALSE, &characterModelViewProj[0][0]));
        glDrawElements(GL_TRIANGLES, character.numIndices, GL_UNSIGNED_INT, nullptr);
        unbind();

        SDL_GL_SwapWindow(window);//switcht die buffer

        GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
        //*loop*//

        //fps
        const double endCounter = static_cast<double>(SDL_GetPerformanceCounter());
        const double counterElapsed = endCounter - lastCounter;
        delta = static_cast<float>(counterElapsed) / static_cast<float>(perfCounterFrequency);
        lastCounter = endCounter;
    }
    return 0;
}