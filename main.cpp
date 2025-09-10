#include "includes.h"

using namespace std;

void openGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam=nullptr) {
#ifdef Release
    if(severity == GL_DEBUG_SEVERITY_HIGH || severity == GL_DEBUG_SEVERITY_MEDIUM) {
        cout << "opengl error message: " << message << endl;
    }
#else
    cout << "opengl debug message: " << message << endl;
#endif
}

int main(int argc, char** argv) {
    SDL_Window* window;
    SDL_Init(SDL_INIT_EVERYTHING);//init sdl
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);//legt buffergröße fest
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

#ifdef Release
    float flags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP;//legt fenster flag fest wie vollbild oder fullscreen
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);//debug modus
    float flags = SDL_WINDOW_OPENGL;
#endif

    window = SDL_CreateWindow("main", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, flags);//setzt die werte für das fenster wie name, position, größe und funktion
    SDL_GLContext glContext = SDL_GL_CreateContext(window);//setzt context für das fenster, damit sdl mit opengl kommunizieren kann, eine art schnittstelle

    GLenum glewError = glewInit();//initalisiert glew, für mehr opengl funktionen und legt es in eine variable ab
    if (glewError != GLEW_OK) {
        cout << "GLEW error: " << glewGetErrorString(glewError) << endl;
        return -1;
    }
#ifndef Release
    cout << "opengl version: " << glGetString(GL_VERSION) << endl;
    glEnable(GL_DEBUG_OUTPUT);//aktiviert debug output
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);//aktiviert die sofortige benachrichtigung
    glDebugMessageCallback(openGLDebugCallback, nullptr);//legt callback fest
#endif

    Vertex vertices[] = {//vertex koordinaten
        Vertex{-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},
        Vertex{-0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f},
        Vertex{0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f},
        Vertex{0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
        Vertex{0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},//für mittleres dreieck
    };
    int numVertices = sizeof(vertices)/sizeof(vertices[0]);//array größe, also anzahl an punkten der dreiecke

    unsigned int indices[] = {//index für vertices
        0, 2, 4//dreieck normal
        //0, 1, 2,//dreieck 1
        //1, 2, 3//dreieck 2
    };
    int numIndices = sizeof(indices)/sizeof(indices[0]);

    IndexBuffer indexBuffer(indices, numIndices, sizeof(indices[0]));//erstellt ein index buffer aufgrundlage der klasse in index_buffer.h
    VertexBuffer vertexBuffer(vertices, numVertices);//erstellt ein vertex buffer aufgrundlage der klasse in vertex_buffer.h

    Shader shader("/home/medusa/projekte/opengl/shaders/basic.vs", "/home/medusa/projekte/opengl/shaders/basic.fs");//erstellt ein shader objekt, das die vertex und fragment shader lädt und kompiliert
    shader.bind();

    int textureWidth;
    int textureHeight;
    int bitsPerPixel;
    stbi_set_flip_vertically_on_load(true);//opengl hat anderes koordinatensystem, smomit müssen wir die x y achse flippe
    auto textureBuffer = stbi_load(textureDir, &textureWidth, &textureHeight, &bitsPerPixel, 4);//lädt die textur
    GLuint textureId;
    GLCALL(glGenTextures(1, &textureId));//erstellt ein texture id mit 1 textur
    GLCALL(glBindTexture(GL_TEXTURE_2D, textureId));//bindet die textur
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));//filtert die größe der textur
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));//filtert die größe der textur
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));//am rand der textur wird abgeschnitten
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));//am rand der textur wird abgeschnitten
    GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer));//bindet die textur
    GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
    if(textureBuffer) {
        stbi_image_free(textureBuffer);
    }

    glm::mat4 model = glm::mat4(1.0f);//matrix zum verschieben vom model, durch das multiplizieren mit der matrix
    model = glm::scale(model, glm::vec3(1.6f));

    int colorUniformLocation = glGetUniformLocation(shader.getShaderId(), "u_in_color");//holt die position der uniform variable im shader programm
    if(colorUniformLocation != -1) {
        glUniform4f(colorUniformLocation, 0.0f, 0.0f, 0.0f, 1.0f);
    }
    else {
        cout << "uniform color not found" << endl;
    }
    int textureUniformLocation = GLCALL(glGetUniformLocation(shader.getShaderId(), "u_texture"));
    if(textureUniformLocation != -1) {
        GLCALL(glUniform1d(textureUniformLocation, 0));
    }
    else {
        cout << "uniform texture not found" << endl;
    }
    int modelMatrixLocation = glGetUniformLocation(shader.getShaderId(), "in_model");//holt die position der uniform variable im shader programm
    if(modelMatrixLocation != -1) {
        GLCALL(glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &model[0][0]));//überträgt die matrix an die uniform variable im shader programm
    }
    else {
        cout << "uniform matrix not found" << endl;
    }

    //fps code
    double perfCounterFrequency = SDL_GetPerformanceFrequency();
    double lastCounter = SDL_GetPerformanceCounter();
    float delta = 0.0f;


    float time = 0.0f;
    bool close = false;
    while(!close) {
        //*loop*//
        glUniform4f(colorUniformLocation, sinf(time)*sinf(time), cosf(time)*cosf(time), tanf(time)*tanf(time), 1.0f);
        GLCALL(glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &model[0][0]));
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);//setzt die clear farbe
        glClear(GL_COLOR_BUFFER_BIT);//cleart in der gesetzen farbe
        vertexBuffer.bind();//zum zeichnen bindet es den vao
        vertexBuffer.bindVbo();//zum neu beschreiben des buffers
        indexBuffer.bind();
        GLCALL(glActiveTexture(GL_TEXTURE0));//aktiviert textur unit 0 um die textur zu binden
        GLCALL(glBindTexture(GL_TEXTURE_2D, textureId));//bindet die textur

        model = glm::rotate(model, delta, glm::vec3(0.0f, 1.0f, 0.0f));
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
        SDL_GL_SwapWindow(window);//switcht die buffer

        vertexBuffer.unbind();
        vertexBuffer.unbindVbo();
        indexBuffer.unbind();
        time += delta;
        //*loop*//

        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                close = true;
            }
        }

        //fps berechnung
        double endCounter = SDL_GetPerformanceCounter();
        double counterElapsed = endCounter - lastCounter;
        delta = ((float)counterElapsed) / (float)perfCounterFrequency;
        float fps = (float)perfCounterFrequency / (float)counterElapsed;
        #ifdef Fps
        cout << "fps: " << (int)fps << endl;
        #endif
        lastCounter = endCounter;
    }
    return 0;
}