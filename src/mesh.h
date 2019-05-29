//
//  mesh.h
//  shader
//
//  Created by jones on 2019/5/2.
//  Copyright © 2019 jones. All rights reserved.
//

#ifndef mesh_h
#define mesh_h
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

struct Mesh {
    std::vector<GLfloat> vertexPositions;
    std::vector<GLfloat> textureCoords;
    std::vector<GLuint>  indices;
};
#endif /* mesh_h */
