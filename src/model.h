//
//  model.h
//  shader
//
//  Created by jones on 2019/5/2.
//  Copyright © 2019 jones. All rights reserved.
//

#ifndef model_h
#define model_h

#include "mesh.h"
#include <vector>

class Model {
public:
    Model() = default;
    Model(const Mesh& mesh);
    ~Model();
    
    void addData(const Mesh& mesh);
    
    void deleteData();
    
    void addVBO(int dimensions, const std::vector<GLfloat>& data);
    void bindVAO() const;
    
    int getIndicesCount() const;
    
private:
    void addEBO(const std::vector<GLuint>& indices);
    
    GLuint vao = 0;
    int vboCount = 0;
    int indicesCount = 0;
    std::vector<GLuint> buffers;
};

#endif /* model_h */
