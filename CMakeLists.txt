cmake_minimum_required(VERSION 3.11)
project(Ex0)

set(CMAKE_CXX_STANDARD 14)

find_package(assimp CONFIG REQUIRED)
find_package(glew REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(sqlite3sqlite3 REQUIRED)

message("assimp: ${ASSIMP_LIBRARIES}")

include_directories(src/glad/include)

set(SOURCES src/main.cpp src/glad/src/glad.c src/stb_image.cpp
        src/Cube.cpp src/MCdb.cpp src/MapManager.cpp)
set(HEADERS src/Shader.h src/stb_image.h src/Camera.h
        src/TextureAtlas.h src/model.h src/mesh.h src/noise.h src/MCdb.h
        src/MapManager.h src/Cube.h src/Texture.h)

add_executable(Ex0 ${SOURCES} ${HEADERS})
target_link_libraries(Ex0 PRIVATE ${ASSIMP_LIBRARIES})
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

add_custom_command(
        TARGET Ex0 PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_SOURCE_DIR}/src/objs
        ${CMAKE_CURRENT_BINARY_DIR}/objs)
