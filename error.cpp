#include "error.h"

#include <cassert>
#include <iostream>

Error::Error() {
    clog << "error initiliazed" << endl << "this should not be normale" << endl;
}

void Error::modelError() {
    cerr << "modelError" << endl;
}

void Error::meshError() {
    cerr << "meshError" << endl;
}

void Error::vertexError() {
    cerr << "vertexError" << endl;
}

void Error::indexError() {
    cerr << "indexError" << endl;
}

void Error::modelNotFound() {
    cerr << "error modelNotFound" << endl;
}

void Error::fatalError() {
    cerr << "fatalError" << endl;
    assert(false);
}

void Error::initError() {
    cerr << "initError" << endl;
    assert(false);
}

void Error::runtimeError() {
    cerr << "runtimeError" << endl;
}

void Error::shaderError() {
    cerr << "shaderError" << endl;
}

void Error::materialError() {
    cerr << "materialError" << endl;
}