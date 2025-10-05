#pragma once
#include <GL/glew.h>

class IndexBuffer final {
public:
    IndexBuffer(const void* data, unsigned int numIndices, int elementSize);
    ~IndexBuffer();
    void bind() const;
    static void unbind();
private:
    GLuint bufferId{};//erstellt ein vertex buffer und eine passenden index
};