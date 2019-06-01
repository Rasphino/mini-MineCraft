#ifndef CUBE_H
#define CUBE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "Shader.h"


class Cube {
private:
    unsigned int VBO_grass, VAO_grass;
    unsigned int VBO_soil, VAO_soil;
    unsigned int VAO_highGrass, VBO_highGrass;
    unsigned int VAO_flower_1, VBO_flower_1;
    unsigned int VAO_flower_2, VBO_flower_2;
    unsigned int VAO_flower_3, VBO_flower_3;
    unsigned int VAO_flower_4, VBO_flower_4;
    unsigned int VAO_flower_5, VBO_flower_5;
    unsigned int VAO_flower_6, VBO_flower_6;
    Shader soilShader;
    Shader grassShader;
    Shader transparentShader;

public:
    Cube();
    void Draw(std::string Name, glm::vec3 pos, glm::mat4 projection, glm::mat4 view);
    ~Cube();
};

#endif

