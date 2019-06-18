#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include "Camera.h"
#include "Cube.h"
#include "MCdb.h"
#include "MapManager.h"
#include "Shader.h"
#include "Texture.h"
#include "TextureAtlas.h"
#include "stb_image.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
// bool checkCollide(direction dir);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
// camera
Camera camera(glm::vec3(30000.0f, 20.0f, 30000.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

float ColorCoefficient = 0.3;
int plusORminus;
typedef struct _VertexData {
    float position[3];
    float texture[2];
} VertexData;

int Chunkvisited[1024] = {0};
int minheight = 100;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MineCraft", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    Cube cube;

    while (!glfwWindowShouldClose(window)) {
        if (ColorCoefficient - 0.3 >= -0.05 && ColorCoefficient - 0.3 <= 0.05) {
            plusORminus = 1;
        } else if (ColorCoefficient - 1.0 <= 0.05 && ColorCoefficient - 1.0 >= -0.05)
            plusORminus = 0;
        if (plusORminus == 1)
            ColorCoefficient += 0.01;
        else
            ColorCoefficient -= 0.01;
        //        cout << ColorCoefficient << endl;

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);
        camera.ProcessFall();
        glClearColor(ColorCoefficient, ColorCoefficient, ColorCoefficient, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection =
                glm::perspective(glm::radians(camera.Zoom), (float) SCR_WIDTH / (float) SCR_HEIGHT,
                                 0.1f, 200.0f);
        glm::mat4 view = camera.GetViewMatrix();

        // 将相机的位置传给MapManager，如果相机移动到另一个chunk，则更新cache
        int x, z;
        mapManager.updateCacheMap(camera.Position);
        // 获取cache的指针，以使用cache进行绘制
        Cache *mapCache = mapManager.getCache();
        std::tie(x, z) = mapManager.getCacheVertexCoord();

        // 遍历cache，根据cache里block的类型，调用cube的Draw函数进行绘制
        for (int cx = 0; cx < CHUNK_NUM; ++cx) {
            for (int cz = 0; cz < CHUNK_NUM; ++cz) {
                for (int i = 0; i < CHUNK_SIZE; i++) {
                    for (int k = 0; k < CHUNK_SIZE; k++) {
                        for (int j = 0; j < 64; j++) {
                            //                            std::cout << "min:" << minheight << endl;
                            glm::vec3 pos = glm::vec3(cx * CHUNK_SIZE + x + i, j,
                                                      cz * CHUNK_SIZE + z + k);
                            //                            std::cout << cx * CHUNK_SIZE + x + i << std::endl;
                            if ((*mapCache)[cx][cz][i][j][k] != CubeType::NONE) {
                                cube.Draw(static_cast<CubeType>((*mapCache)[cx][cz][i][j][k]),
                                          pos,
                                          projection,
                                          view,
                                          camera.Position);
                            }
                        }
                    }
                }
            }
        }

        glm::vec3 pos = camera.Position + camera.Front;
        cube.Draw(CubeType::SOIL, pos, projection, view, camera.Position);

        glUseProgram(0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !camera.checkCollide(front))
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && !camera.checkCollide(back))
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && !camera.checkCollide(goleft))
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && !camera.checkCollide(goright))
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !camera.checkCollide(up))
        camera.ProcessKeyboard(JUMP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) camera.ProcessKeyboard(TAB, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) camera.ProcessKeyboard(DOWN, deltaTime);
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
