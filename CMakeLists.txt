cmake_minimum_required(VERSION 4.0)
project(opengl)

set(CMAKE_CXX_STANDARD 14)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

add_executable(
        opengl main.cpp
        vertex_buffer.h
        vertex_buffer.cpp
        index_buffer.h
        index_buffer.cpp
        defines.h
        shader.h
        shader.cpp
        font.h
        font.cpp
        camera.h
        camera.cpp
        mesh.h
        mesh.cpp
        control.h
        control.cpp
        weapon.h
        weapon.cpp
        model.h
        model.cpp
        level.h
        level.cpp
        enemy.h
        enemy.cpp
        projektil.h
        projektil.cpp
        character.h
        character.cpp
        setVariables.h
        setVariables.cpp)

add_executable(
        modelexporter tools/modelexporter.cpp)

target_link_libraries(opengl
        ${OPENGL_LIBRARIES}
        ${SDL2_LIBRARIES}
        ${GLEW_LIBRARIES}
        glfw
        assimp::assimp
        SDL2_ttf
)

target_link_libraries(modelexporter
        ${OPENGL_LIBRARIES}
        ${SDL2_LIBRARIES}
        ${GLEW_LIBRARIES}
        glfw
        assimp::assimp
)