#include "includes.h"
#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"
#include <vector>
#include <fstream>
#include <iostream>

void openGL_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam=nullptr) {
#ifdef Release
    if(severity == GL_DEBUG_SEVERITY_HIGH || severity == GL_DEBUG_SEVERITY_MEDIUM) {
        cout << "opengl error message: " << message << endl;
    }
#else
    cout << "opengl debug message: " << message << endl;
#endif
}

void debug(int n) {
#ifndef Release
    if(n == 1) {
        cout << "uniform color not found" << endl;
    }
    else if(n == 2) {
        cout << "uniform texture not found" << endl;
    }
    else if(n == 3) {
        cout << "uniform modelviewproj not found" << endl;
    }
    else if(n == 4) {
        cout << "error reading model file .mds" << endl;
    }
#endif
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
    const GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        cout << "GLEW error: " << glewGetErrorString(glewError) << endl;
        return -1;
    }

    //debug modus
#ifndef Release
    cout << "opengl version: " << glGetString(GL_VERSION) << endl;
    glEnable(GL_DEBUG_OUTPUT);//aktiviert debug output
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);//aktiviert die sofortige benachrichtigung
    glDebugMessageCallback(openGL_debug_callback, nullptr);//legt callback fest
#endif

    //koordinaten
    vector<Vertex> vertices;
    uint64_t numVertices;
    vector<uint32_t> indices;
    int64_t numIndices;
    ifstream input = ifstream(modelDir, ios::in | ios::binary);
    if(!input.is_open()) {
        debug(4);
    }
    input.read((char*)&numVertices, sizeof(uint64_t));
    input.read((char*)&numIndices, sizeof(uint64_t));

    for(uint64_t i = 0; i < numVertices; i++) {
        Vertex vertex{};
        input.read((char*)&vertex.x, sizeof(float));
        input.read((char*)&vertex.y, sizeof(float));
        input.read((char*)&vertex.z, sizeof(float));
        vertex.u = 1.0f;
        vertex.v = 1.0f;
        vertex.r = 1.0f;
        vertex.g = 1.0f;
        vertex.b = 1.0f;
        vertex.a = 1.0f;
        vertices.push_back(vertex);
    }
    for(uint64_t i = 0; i < numIndices; i++) {
        uint32_t index;
        input.read((char*)&index, sizeof(uint32_t));
        indices.push_back(index);
    }

    //erstellt indexbuffer, vertexbuffer und erstellt die shader programme für die gpu
    const VertexBuffer vertexBuffer(vertices.data(), numVertices);
    const IndexBuffer indexBuffer(indices.data(), numIndices, sizeof(indices[0]));
    const Shader shader(vertexShaderDir, fragmentShaderDir);

    //kamera
#ifndef fpsMode
    Camera camera(90.0f, windowWidth, windowHeight);
#else
    FPSCamera camera(90.0f, windowWidth, windowHeight);
#endif
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
    glm::mat4 model = glm::mat4(1.0f);//matrix zur positionierung der vertices
    model = glm::scale(model, glm::vec3(1.0f));
    glm::mat4 modelViewProj = projection * model;//gesamte matrix zur vereinfachung um es an den shader zu übergeben

    //holt sich variablen aus dem shader um deren speicherort zu speichern um die daten darin zu ändern
    const int colorUniformLocation = glGetUniformLocation(shader.getShaderId(), "u_in_color");
    if(colorUniformLocation != -1) {
        glUniform4f(colorUniformLocation, 0.0f, 0.0f, 0.0f, 1.0f);
    }
    else {
        debug(1);
    }
    const int textureUniformLocation = GLCALL(glGetUniformLocation(shader.getShaderId(), "u_in_texture"));
    if(textureUniformLocation != -1) {
        GLCALL(glUniform1d(textureUniformLocation, 0));
    }
    else {
        debug(2);
    }
    const int modelViewProjLocation = glGetUniformLocation(shader.getShaderId(), "u_in_model_view_proj");
    if(modelViewProjLocation != -1) {
        GLCALL(glUniformMatrix4fv(modelViewProjLocation, 1, GL_FALSE, &modelViewProj[0][0]));
    }
    else {
        debug(3);
    }

    //FPS
    const double perfCounterFrequency = static_cast<double>(SDL_GetPerformanceFrequency());
    double lastCounter = static_cast<double>(SDL_GetPerformanceCounter());
    float delta = 0.0f;
    float time = 0.0f;

    shader.bind();

    bool close = false;

    //tasten
    bool wBool = false;
    bool sBool = false;
    bool aBool = false;
    bool dBool = false;
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
                }
            }
        }

#ifndef fpsMode
        //kamerasteuerung
        if(wBool) {
            camera.translate(glm::vec3(0.0f, 0.0f, -0.05f));
        }
        if(sBool) {
            camera.translate(glm::vec3(0.0f, 0.0f, 0.05f));
        }
        if(aBool) {
            camera.translate(glm::vec3(-0.03f, 0.0f, 0.0f));
        }
        if(dBool) {
            camera.translate(glm::vec3(0.03f, 0.0f, 0.0f));
        }
#else
        //kamerasteuerung
        SDL_SetRelativeMouseMode(SDL_TRUE);
        if(wBool) {
            camera.moveFront(delta * cameraSpeed);
        }
        if(sBool) {
            camera.moveFront(-delta * cameraSpeed);
        }
        if(aBool) {
            camera.moveSideways(-delta * cameraSpeed);
        }
        if(dBool) {
            camera.moveSideways(delta * cameraSpeed);
        }
        if(event.type == SDL_MOUSEMOTION) {
            if(SDL_GetRelativeMouseMode()) {
                event.motion.xrel *= 0.9f;
                event.motion.yrel *= 0.9f;
                camera.onMouseMove(event.motion.xrel, event.motion.yrel);
            }
        }
#endif
        camera.update();
        projection = camera.getViewProjection();

        glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);//cleart den zu bearbeitenden buffer
        vertexBuffer.bind();//hat die vertex daten gespeichert
        vertexBuffer.bindVbo();//zum neu beschreiben des buffers, zB. wenn man in der laufzeit die vertices ändert
        indexBuffer.bind();//hat den index der vertices gespeichert
        GLCALL(glActiveTexture(GL_TEXTURE0));//aktiviert textur unit 0 um die textur zu binden
        GLCALL(glBindTexture(GL_TEXTURE_2D, textureId));//bindet die textur
        glUniform4f(colorUniformLocation, 0.5f, 0.0f, 1.0f, 1.0f);

        time += delta;
        model = glm::rotate(model, delta, glm::vec3(1.0f, 1.0f, 1.0f));
        modelViewProj = projection * model;
        GLCALL(glUniformMatrix4fv(modelViewProjLocation, 1, GL_FALSE, &modelViewProj[0][0]));//ändert die daten in der modelViewPorjLocation im shader
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
        SDL_GL_SwapWindow(window);//switcht die buffer

        VertexBuffer::unbind();
        VertexBuffer::unbindVbo();
        IndexBuffer::unbind();
        GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
        //*loop*//

        //fps
        const double endCounter = static_cast<double>(SDL_GetPerformanceCounter());
        const double counterElapsed = endCounter - lastCounter;
        delta = static_cast<float>(counterElapsed) / static_cast<float>(perfCounterFrequency);
#ifdef Fps
        const float fps = static_cast<float>(perfCounterFrequency) / static_cast<float>(counterElapsed);
        cout << "fps: " << static_cast<int>(fps) << endl;
#endif
        lastCounter = endCounter;
    }
    return 0;
}