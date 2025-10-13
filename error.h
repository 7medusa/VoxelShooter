#pragma once

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
};