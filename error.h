#pragma once
#include <string>

using namespace std;

class Error {
public:
    Error();
    static void modelError();
    static void meshError();
    static void vertexError();
    static void indexError();
    static void modelNotFound();
    static void fatalError();
    static void initError();
    static void runtimeError();
    static void shaderError(string source);
    static void materialError();
    static void uniformError();
};