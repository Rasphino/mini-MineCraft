cmake_minimum_required(VERSION 3.11)
project(Ex0)

set(CMAKE_CXX_STANDARD 14)

find_package(assimp REQUIRED)
find_package(glew REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(sqlite3 REQUIRED)

include_directories(src/glad/include)

set(SOURCES src/main.cpp src/glad/src/glad.c src/stb_image.cpp
        src/db/Hash.cpp src/db/MCdb.cpp src/Cube.cpp)
set(HEADERS src/Shader.h src/stb_image.h src/Camera.h
        src/TextureAtlas.h src/model.h src/mesh.h src/noise.h src/db/MCdb.h
        src/db/Hash.h src/Cube.h)
set(TEXTURES src/textures/Test.png src/shaders/texture.fs src/shaders/texture.vs)

add_executable(Ex0 ${SOURCES} ${HEADERS})

target_link_libraries(Ex0 PRIVATE assimp)
target_link_libraries(Ex0 PRIVATE GLEW::GLEW)
target_link_libraries(Ex0 PRIVATE glfw)
target_link_libraries(Ex0 PRIVATE glm)
target_link_libraries(Ex0 PRIVATE sqlite3)

add_custom_command(
        TARGET Ex0 PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_SOURCE_DIR}/src/shaders
        ${CMAKE_CURRENT_BINARY_DIR}/shaders)

add_custom_command(
        TARGET Ex0 PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_SOURCE_DIR}/src/textures
        ${CMAKE_CURRENT_BINARY_DIR}/textures)
