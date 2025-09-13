#include "defines.h"

class VertexBuffer {
private:
    GLuint bufferId;//erstellt ein vertex buffer und eine passenden index
    GLuint vao;//Vertex Array Object, das die vertex buffer und index buffer zusammenfasst, damit opengl sie als ein objekt behandeln kann
public:
    VertexBuffer(void* data, unsigned int numVertices) {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &bufferId);//generiert ein buffer der die vertices enthält
        glBindBuffer(GL_ARRAY_BUFFER, bufferId);
        glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), data, GL_STATIC_DRAW);//räumt speicher für die vertices frei
        glEnableVertexAttribArray(0);//aktiviert den ersten attribut
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);//legt fest, wie die punkte des dreiecks im vertex buffer gespeichert sind, damit opengl sie richtig interpretieren kann
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(struct Vertex, u));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(struct Vertex, r));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        //buffer speichert
        //vao speichert wie die daten gelesen werden und an den shader übergeben werden
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