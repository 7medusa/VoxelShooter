#include "includes.h"

using namespace std;

void openGL_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam=nullptr) {
#ifdef Release
    if(severity == GL_DEBUG_SEVERITY_HIGH || severity == GL_DEBUG_SEVERITY_MEDIUM) {
        cout << "opengl error message: " << message << endl;
    }
#else
    cout << "opengl debug message: " << message << endl;
#endif
}

void uniforms(Shader shader, glm::mat4 modelViewProj) {
    int colorUniformLocation = glGetUniformLocation(shader.getShaderId(), "u_in_color");
    if(colorUniformLocation != -1) {
        glUniform4f(colorUniformLocation, 0.0f, 0.0f, 0.0f, 1.0f);
    }
    else {
        cout << "uniform color not found" << endl;
    }
    int textureUniformLocation = GLCALL(glGetUniformLocation(shader.getShaderId(), "u_in_texture"));
    if(textureUniformLocation != -1) {
        GLCALL(glUniform1d(textureUniformLocation, 0));
    }
    else {
        cout << "uniform texture not found" << endl;
    }
    int modelViewProjLocation = glGetUniformLocation(shader.getShaderId(), "u_in_model_view_proj");
    if(modelViewProjLocation != -1) {
        GLCALL(glUniformMatrix4fv(modelViewProjLocation, 1, GL_FALSE, &modelViewProj[0][0]));
    }
    else {
        cout << "uniform modelviewproj not found" << endl;
    }
}

int main(int argc, char** argv) {
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
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);//debug modus
    int flags = SDL_WINDOW_OPENGL;
#endif

    //erstellt und definiert eigenschaften für ein fenster
    SDL_Window* window;
    window = SDL_CreateWindow("main", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, flags);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);//setzt ein kontext damit opengl mit dem window manager sdl kommunizieren kann

    //initialisiert glew für mehr funktionen
    GLenum glewError = glewInit();
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
    Vertex vertices[] = {
        Vertex{-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},
        Vertex{-0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f},
        Vertex{0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f},
        Vertex{0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
        Vertex{0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},//für mittleres dreieck
    };
    int numVertices = sizeof(vertices)/sizeof(vertices[0]);//array größe

    unsigned int indices[] = {
        //0, 2, 4//dreieck normal
        0, 1, 2,//dreieck 1
        1, 2, 3//dreieck 2
    };
    int numIndices = sizeof(indices)/sizeof(indices[0]);//array größe

    //erstellt indexbuffer, vertexbuffer und erstellt die shader programme für die gpu
    IndexBuffer indexBuffer(indices, numIndices, sizeof(indices[0]));
    VertexBuffer vertexBuffer(vertices, numVertices);
    Shader shader("/home/medusa/projekte/opengl/shaders/basic.vs", "/home/medusa/projekte/opengl/shaders/basic.fs");

    //textur
    int textureWidth;
    int textureHeight;
    int bitsPerPixel;
    stbi_set_flip_vertically_on_load(true);//flippt das koordinatensystem da opengl ein anderes system nutzt
    auto textureBuffer = stbi_load(textureDir, &textureWidth, &textureHeight, &bitsPerPixel, 4);//lädt die textur
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
    glm::mat4 model = glm::mat4(1.0f);//matrix zur positionierung der vertices
    model = glm::scale(model, glm::vec3(2.0f));
    glm::mat4 projection = glm:: perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);//matrix zur perspektive
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -6.0f));//matrix um die kamera zu verschieben
    glm::mat4 modelViewProj = projection * view * model;//gesamte matrix zur vereinfachung als uniform

    //holt sich variablen aus dem shader um deren speicherort zu speichern um die daten darin zu ändern
    int colorUniformLocation = glGetUniformLocation(shader.getShaderId(), "u_in_color");
    if(colorUniformLocation != -1) {
        glUniform4f(colorUniformLocation, 0.0f, 0.0f, 0.0f, 1.0f);
    }
    else {
        cout << "uniform color not found" << endl;
    }
    int textureUniformLocation = GLCALL(glGetUniformLocation(shader.getShaderId(), "u_in_texture"));
    if(textureUniformLocation != -1) {
        GLCALL(glUniform1d(textureUniformLocation, 0));
    }
    else {
        cout << "uniform texture not found" << endl;
    }
    int modelViewProjLocation = glGetUniformLocation(shader.getShaderId(), "u_in_model_view_proj");
    if(modelViewProjLocation != -1) {
        GLCALL(glUniformMatrix4fv(modelViewProjLocation, 1, GL_FALSE, &modelViewProj[0][0]));
    }
    else {
        cout << "uniform modelviewproj not found" << endl;
    }

    //FPS
    double perfCounterFrequency = SDL_GetPerformanceFrequency();
    double lastCounter = SDL_GetPerformanceCounter();
    float delta = 0.0f;
    float time = 0.0f;

    shader.bind();
    bool close = false;
    while(!close) {
        //*loop*//
        GLCALL(glUniformMatrix4fv(modelViewProjLocation, 1, GL_FALSE, &modelViewProj[0][0]));//ändert die daten in der modelViewPorjLocation
        glClearColor(0.5f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);//cleart den zu bearbeitenden buffer
        vertexBuffer.bind();//hat die vertex daten gespeichert
        vertexBuffer.bindVbo();//zum neu beschreiben des buffers, zB. wenn man in der laufzeit die vertices ändert
        indexBuffer.bind();//hat den index der vertices gespeichert
        GLCALL(glActiveTexture(GL_TEXTURE0));//aktiviert textur unit 0 um die textur zu binden
        GLCALL(glBindTexture(GL_TEXTURE_2D, textureId));//bindet die textur

        time += delta;
        model = glm::rotate(model, delta, glm::vec3(0.0f, 1.0f, 0.0f));
        modelViewProj = projection * view * model;
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
        SDL_GL_SwapWindow(window);//switcht die buffer

        vertexBuffer.unbind();
        vertexBuffer.unbindVbo();
        indexBuffer.unbind();
        GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
        //*loop*//

        //fps
        double endCounter = SDL_GetPerformanceCounter();
        double counterElapsed = endCounter - lastCounter;
        delta = ((float)counterElapsed) / (float)perfCounterFrequency;
        float fps = (float)perfCounterFrequency / (float)counterElapsed;
#ifdef Fps
        cout << "fps: " << (int)fps << endl;
#endif
        lastCounter = endCounter;

        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                close = true;
            }
        }
    }
    return 0;
}