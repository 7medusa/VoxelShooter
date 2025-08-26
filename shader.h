#pragma once
#include "defines.h"
#include <GL/glew.h>
#include <string>

using namespace std;

class Shader {
public:
    Shader(const char* vertexShaderFilename, const char* fragmentShaderFilename);
    virtual ~Shader();

    void bind();
    void unbind();

private:
    string parse(const char* filename);
    GLuint createShader(const char* vertexShaderFilename, const char* fragmentShaderFilename);
    GLuint compile(string shaderSource, GLenum type);

    GLuint shaderId;
};