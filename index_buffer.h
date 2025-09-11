#pragma once
#include <GL/glew.h>
#include "defines.h"

class IndexBuffer {
private:
    GLuint bufferId;//erstellt ein vertex buffer und eine passenden index
public:
    IndexBuffer(void* data, unsigned int numIndices, int elementSize) {
        glGenBuffers(1, &bufferId);//erzeugt einen indexbuffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * elementSize, data, GL_STATIC_DRAW);//räumt speicher für die indices frei
        glEnableVertexAttribArray(0);//aktiviert den ersten attributen im vertex shader, in dem fall die position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);//legt fest, wie die punkte des dreiecks im vertex buffer gespeichert sind, damit opengl sie richtig interpretieren kann
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    virtual ~IndexBuffer() {
        glDeleteBuffers(1, &bufferId);
    }
    void bind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
    }
    void unbind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
};