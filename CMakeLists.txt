cmake_minimum_required(VERSION 3.11)
project(Ex0)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/glad/include)

set(SOURCES src/main.cpp src/glad/src/glad.c src/stb_image.cpp
        src/model.cpp src/db/Hash.cpp src/db/MCdb.cpp)
set(HEADERS src/Shader.h src/stb_image.h src/Camera.h src/RenderCube
        src/TextureAtlas.h src/model.h src/mesh.h src/noise.h src/db/MCdb.h
        src/db/Hash.h)
set(TEXTURES src/textures/Test.png src/shaders/texture.fs src/shaders/texture.vs)

add_executable(Ex0 ${SOURCES} ${HEADERS})

# glfw
add_subdirectory(lib/glfw EXCLUDE_FROM_ALL)
target_link_libraries(Ex0 PRIVATE glfw)

# glew
add_definitions(-DGLEW_STATIC)
add_subdirectory(lib/glew EXCLUDE_FROM_ALL)
target_link_libraries(Ex0 PRIVATE libglew_static)

# glm
add_subdirectory(lib/glm EXCLUDE_FROM_ALL)
target_link_libraries(Ex0 PRIVATE glm)

# assimp
add_subdirectory(lib/assimp EXCLUDE_FROM_ALL)
target_link_libraries(Ex0 PRIVATE assimp)

# sqlite
add_subdirectory(lib/sqlite EXCLUDE_FROM_ALL)
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
