#include "error.h"

#include <cassert>
#include <iostream>

Error::Error() {
    clog << "error initiliazed" << endl << "this should not be normale" << endl;
}

void Error::modelError() {
    cerr << "error modelError" << endl;
}

void Error::meshError() {
    cerr << "error meshError" << endl;
}

void Error::vertexError() {
    cerr << "error vertexError" << endl;
}

void Error::indexError() {
    cerr << "error indexError" << endl;
}

void Error::modelNotFound() {
    cerr << "modelNotFound" << endl;
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