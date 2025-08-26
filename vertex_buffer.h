#pragma once
#include <GL/glew.h>
#include "defines.h"

class VertexBuffer {
private:
    GLuint bufferId;//erstellt ein vertex buffer und eine passenden index
    GLuint vao;//Vertex Array Object, das die vertex buffer und index buffer zusammenfasst, damit opengl sie als ein objekt behandeln kann
public:
    VertexBuffer(void* data, unsigned int numVertices) {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &bufferId);//generiert den vertex buffer, der die punkte des dreiecks enthält und erzeugt ein index
        glBindBuffer(GL_ARRAY_BUFFER, bufferId);//bindet den vertex buffer an die aktuelle opengl kontext, damit er benutzt werden kann
        glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), data, GL_STATIC_DRAW);//numVertices als anzahl der punkte und Vertex als speicherplatz der punkte
        glEnableVertexAttribArray(0);//aktiviert den ersten attributen im vertex shader, in dem fall die position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);//legt fest, wie die punkte des dreiecks im vertex buffer gespeichert sind, damit opengl sie richtig interpretieren kann

        glEnableVertexAttribArray(1);//aktiviert den zweiten attributen im vertex shader, in dem fall die farbe
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(struct Vertex, r));//legt fest, wie die punkte des dreiecks im vertex buffer gespeichert sind, damit opengl sie richtig interpretieren kann

        glBindBuffer(GL_ARRAY_BUFFER, 0);//unbinded alle buffer
        glBindVertexArray(0);//unbinded alle vao
    }
    virtual ~VertexBuffer() {
        glDeleteBuffers(1, &bufferId);
    }
    void bind() {
        glBindVertexArray(vao);//bindet den vao, indem der vertex buffer und index buffer zusammengefasst werden, damit opengl sie als ein objekt behandeln kann
    }
    void unbind() {
        glBindVertexArray(0);
    }
    void bindVbo() {//zum updaten der vertices
        glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    }
    void unbindVbo() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};