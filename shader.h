#pragma once

using namespace std;

class Shader final {
public:
    Shader(const char* vertexShaderFilename, const char* fragmentShaderFilename);
    ~Shader();

    void bind() const;
    static void unbind();
    GLuint getShaderId() const {return shaderId;}

private:
    static string parse(const char* filename);
    static GLuint createShader(const char* vertexShaderFilename, const char* fragmentShaderFilename);
    static GLuint compile(const string& shaderSource, GLenum type);

    GLuint shaderId;
};