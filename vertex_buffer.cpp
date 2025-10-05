#include "vertex_buffer.h"
#include <GL/glew.h>
#include <cstddef>
#include "defines.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int numVertices) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &bufferId);//generiert ein buffer der die vertices enthält
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), data, GL_STATIC_DRAW);//räumt speicher für die vertices frei NOLINT(*-narrowing-conversions)

    glEnableVertexAttribArray(0);//aktiviert den ersten attribut
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));//legt fest, wie die punkte des dreiecks im vertex buffer gespeichert sind, damit opengl sie richtig interpretieren kann
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, textureCoord)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //buffer speichert
    //vao speichert wie die daten gelesen werden und an den shader übergeben werden
}
VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &bufferId);
}
void VertexBuffer::bind() const {
    glBindVertexArray(vao);//bindet den vao, indem der vertex buffer und index buffer zusammengefasst werden, damit opengl sie als ein objekt behandeln kann
}
void VertexBuffer::unbind() {
    glBindVertexArray(0);
}
void VertexBuffer::bindVbo() const {//zum updaten der vertices
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
}
void VertexBuffer::unbindVbo() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}