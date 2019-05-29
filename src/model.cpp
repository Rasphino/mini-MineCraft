//
//  Model.cpp
//  shader
//
//  Created by jones on 2019/5/2.
//  Copyright © 2019 jones. All rights reserved.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "model.h"
Model::Model(const Mesh& mesh) {
    addData(mesh);
}

Model::~Model() {
    deleteData();
}

void Model::bindVAO() const {
    glBindVertexArray(vao);
}

void Model::addData(const Mesh& mesh) {
    if (vao != 0)
        deleteData();
    
    indicesCount = mesh.indices.size();
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    addVBO(3, mesh.vertexPositions);
    addVBO(2, mesh.textureCoords);
    addEBO(mesh.indices);
}

void Model::addVBO(int dimensions, const std::vector<GLfloat>& data) {
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 data.size() * sizeof(GLfloat),
                 data.data(),
                 GL_STATIC_DRAW);
    
    glVertexAttribPointer(vboCount,
                          dimensions,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          (GLvoid*) 0);
    
    glEnableVertexAttribArray(vboCount++);
    
    buffers.push_back(vbo);
}

void Model::addEBO(const std::vector<GLuint>& indices) {
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(GLuint),
                 indices.data(),
                 GL_STATIC_DRAW);
}

void Model::deleteData() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(buffers.size(),
                    buffers.data());
    
    buffers.clear();
    
    vboCount      = 0;
    vao           = 0;
    indicesCount  = 0;
}

int Model::getIndicesCount() const {
    return indicesCount;
}
