    //debug modus
#ifndef Release
    cout << "opengl version: " << glGetString(GL_VERSION) << endl;
glEnable(GL_DEBUG_OUTPUT);//aktiviert debug output
glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);//aktiviert die sofortige benachrichtigung
glDebugMessageCallback(openGLDebugCallback, nullptr);//legt callback fest
#endif