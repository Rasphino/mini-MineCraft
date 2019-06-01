#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <iostream>
#include <string>
#include <vector>
#include <tuple>

#include <assimp/scene.h>

#include "stb_image.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "TextureAtlas.h"
#include "MCdb.h"
#include "MapManager.h"
#include "Cube.h"
#include "model.h"

using namespace std;
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
// camera
Camera camera(glm::vec3(30000.0f, 20.0f, 30000.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

// timing
float deltaTime = 0.0f;//time between current frame and last frame
float lastFrame = 0.0f;

typedef struct _VertexData {
    float position[3];
    float texture[2];
} VertexData;

int Chunkvisited[1024] = {0};

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    std::string file = "textures/texture.png";
    TextureAtlas tex(file);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    MapManager mapManager(camera.Position);

    Model firTree("objs/cat/Cat.obj");
    Shader modelShader("shaders/model.vs", "shaders/model.fs");
    Cube cube;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        modelShader.use();
        // view/projection transformations
        glm::mat4 projection_model = glm::perspective(glm::radians(camera.Zoom),
                                                      (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f,
                                                      100.0f);
        glm::mat4 view_model = camera.GetViewMatrix();

        modelShader.setMat4("projection", projection_model);
        modelShader.setMat4("view", view_model);

        // render the loaded model
        glm::mat4 model_ = glm::mat4(1.0f);
        model_ = glm::translate(model_, glm::vec3(0.0f, 10.0f,
                                                  0.0f)); // translate it down so it's at the center of the scene
        model_ = glm::scale(model_, glm::vec3(0.01f, 0.01f,
                                              0.01f));    // it's a bit too big for our scene, so scale it down
        model_ = glm::rotate(model_, -1.57f, glm::vec3(1, 0, 0));
        modelShader.setMat4("model", model_);
        firTree.Draw(modelShader);

        tex.TexActivate();
        tex.BindTexture();
        camera.ProcessJump();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                                                (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f,
                                                200.0f);
        glm::mat4 view = camera.GetViewMatrix();

        int x, z;
        mapManager.updateCacheMap(camera.Position);
        Cache *mapCache = mapManager.getCache();
        std::tie(x, z) = mapManager.getCacheVertexCoord();

        for (int cx = 0; cx < 3; ++cx) {
            for (int cz = 0; cz < 3; ++cz) {
                for (int i = 0; i < 16; i++) {
                    for (int k = 0; k < 16; k++) {
                        for (int j = 0; j < 64; j++) {
                            glm::vec3 pos = glm::vec3(cx * 16 + x + i, j, cz * 16 + z + k);
                            switch ((*mapCache)[cx][cz][i][j][k]) {
                                case MapManager::BlockType::SOIL: {
                                    cube.Draw("soil", pos, projection, view);
                                    break;
                                }
                                case MapManager::BlockType::GRASS: {
                                    cube.Draw("grass", pos, projection, view);
                                    break;
                                }
                                case MapManager::BlockType::HIGHGRASS: {
                                    cube.Draw("highGrass", pos, projection, view);
                                    break;
                                }
                                case MapManager::BlockType::FLOWER_1: {
                                    cube.Draw("flower_1", pos, projection, view);
                                    break;
                                }
                                case MapManager::BlockType::FLOWER_2: {
                                    cube.Draw("flower_2", pos, projection, view);
                                    break;
                                }
                                case MapManager::BlockType::FLOWER_3: {
                                    cube.Draw("flower_3", pos, projection, view);
                                    break;
                                }
                                case MapManager::BlockType::FLOWER_4: {
                                    cube.Draw("flower_4", pos, projection, view);
                                    break;
                                }
                                case MapManager::BlockType::FLOWER_5: {
                                    cube.Draw("flower_5", pos, projection, view);
                                    break;
                                }
                                case MapManager::BlockType::FLOWER_6: {
                                    cube.Draw("flower_6", pos, projection, view);
                                    break;
                                }

                                default:
                                case MapManager::BlockType::NONE: {

                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

        glUseProgram(0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(JUMP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
        camera.ProcessKeyboard(TAB, deltaTime);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}
