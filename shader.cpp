#include "includes.h"

Shader::Shader(const char* vertexShaderFilename, const char* fragmentShaderFilename) {
    shaderId = createShader(vertexShaderFilename, fragmentShaderFilename);
}
Shader::~Shader() {
    glDeleteProgram(shaderId);
}

void Shader::bind() {
    glUseProgram(shaderId);
}
void Shader::unbind() {
    glUseProgram(0);
}

GLuint Shader::compile(string shaderSource, GLenum type) {
    GLuint id = glCreateShader(type);
    const char* src = shaderSource.c_str();
    glShaderSource(id, 1, &src, 0);
    glCompileShader(id);

    //fehlerbehandlung des shaders
    int result = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE) {
        cout << "shader compile error" << endl;
        return 0;
    }
#ifndef Release
    cout << "shader compile success" << endl;
#endif
    return id;
}
string Shader::parse(const char* filename) {//liest die file aus
    FILE* file;
    string contents;
    file = fopen(filename, "rb");
    if(file == nullptr) {
        cout << "error opening shader file: " << filename << endl;
        return "";//gibt einen leeren shader zurück, wenn die datei nicht geöffnet werden kann
    }
    fseek(file, 0, SEEK_END);
    size_t filesize = ftell(file);
    rewind(file);
    contents.resize(filesize);
    fread(&contents[0], 1, filesize, file);
    fclose(file);
    return contents;
}
GLuint Shader::createShader(const char* vertexShaderFilename, const char* fragmentShaderFilename) {
    string vertexShaderSource = parse(vertexShaderFilename);
    string fragmentShaderSource = parse(fragmentShaderFilename);

    GLuint program = glCreateProgram();
    GLuint vs = compile(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fs = compile(fragmentShaderSource, GL_FRAGMENT_SHADER);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glDetachShader(program, vs);
    glDetachShader(program, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

/*
Shader-Quelltext einlesen: Der GLSL-Code des Shaders (Vertex- oder Fragment-Shader) wird als Text in dein Programm geladen.

Shader-Objekt erstellen: Mit glCreateShader(type) erzeugst du ein leeres Shader-Objekt im OpenGL-Kontext, das bereit ist, den Quelltext aufzunehmen.

Quelltext an Shader-Objekt übergeben: glShaderSource verbindet deinen geladenen GLSL-Quelltext mit dem Shader-Objekt.

Shader kompilieren: Mit glCompileShader wird der Quelltext in eine für die Grafikkarte ausführbare Form übersetzt.

Fehlerüberprüfung: Nach der Kompilierung prüfst du mit glGetShaderiv und GL_COMPILE_STATUS, ob die Kompilierung erfolgreich war. Falls nicht, kannst du den Fehler behandeln.

Shader nach erfolgreicher Kompilierung verlinken: Die kompilierten Shader-Objekte (zum Beispiel Vertex-Shader und Fragment-Shader) werden mit einem Shader-Programm (mittels glCreateProgram) zusammengeführt und verlinkt. Dieses Programm kannst du dann verwenden, um deine Rendering-Pipeline zu steuern.

Shaderprogramm zur Laufzeit verwenden: Das verlinkte Shaderprogramm wird während des Renderns aktiviert und verrichtet seine Arbeit direkt auf der GPU, indem es die Berechnungen für die Verarbeitung von Vertices bzw. Fragmenten ausführt.
*/