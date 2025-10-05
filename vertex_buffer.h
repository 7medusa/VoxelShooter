#pragma once
#include <GL/glew.h>

class VertexBuffer final {
public:
    VertexBuffer(const void* data, unsigned int numVertices);
    ~VertexBuffer();
    void bind() const;
    static void unbind();
    void bindVbo() const;
    static void unbindVbo();
private:
    GLuint bufferId{};//erstellt ein vertex buffer und eine passenden index
    GLuint vao{};//Vertex Array Object, das die vertex buffer und index buffer zusammenfasst, damit opengl sie als ein objekt behandeln kann
};