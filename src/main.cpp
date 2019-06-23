#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <map>

#include "Camera.h"
#include "Cube.h"
#include "MCdb.h"
#include "MapManager.h"
#include "Shader.h"
#include "Texture.h"
#include "TextureAtlas.h"
#include "Sky.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
unsigned int loadCubemapsTexture(const std::vector<std::string> &fullpathVec);
//bool checkCollide(direction dir);

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

float ColorCoefficient = 0.3;
int plusORminus;

struct VertexData {
    float position[3];
    float texture[2];
};

int Chunkvisited[1024] = {0};
Sky sky;

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

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    std::string file = "textures/texture.png";
    TextureAtlas tex(file);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(2.0f);

    MapManager mapManager(camera.Position);

    Cube cube;
    Shader skyboxShader("shaders/skyShader.vs", "shaders/skyShader.fs");
    GLUquadricObj *qobj = gluNewQuadric();
    gluQuadricNormals(qobj, GLU_SMOOTH);
    gluQuadricTexture(qobj, GL_TRUE);

    float skyboxVertices[] = {
            // 位置
            -1.0f, 1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            -1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f
    };


    //天空盒VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(8);
    glVertexAttribPointer(8, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);


    //加载纹理盒
    std::vector<std::string> fullpathVec;
    fullpathVec.push_back("textures/right_.jpg");
    fullpathVec.push_back("textures/left_.jpg");
    fullpathVec.push_back("textures/top_.jpg");
    fullpathVec.push_back("textures/botton_.jpg");
    fullpathVec.push_back("textures/back_3.jpg");
    fullpathVec.push_back("textures/front_3.jpg");
    unsigned int cubemapTexture = loadCubemapsTexture(fullpathVec);

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    sky.InitSky(30000.0f, 20.0f, 30000.0f, 0.5, cubemapTexture);
    float r = 0;

    while (!glfwWindowShouldClose(window)) {
//        glEnable(GL_ALPHA);
//        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if (ColorCoefficient - 0.3 >= -0.05 && ColorCoefficient - 0.3 <= 0.05) {
            plusORminus = 1;
        } else if (ColorCoefficient - 1.0 <= 0.05 && ColorCoefficient - 1.0 >= -0.05)
            plusORminus = 0;
        if (plusORminus == 1)
            ColorCoefficient += 0.01;
        else
            ColorCoefficient -= 0.01;

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glDrawElements(GL_POINT, 1, GL_UNSIGNED_INT, 0);

        processInput(window);
        camera.ProcessFall();

        glClearColor(ColorCoefficient, ColorCoefficient, ColorCoefficient, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDepthMask(GL_FALSE);
        sky.ShowSky();
        r += 1;
        glm::mat4 viewskybox = camera.GetViewMatrix();
        glm::mat4 projectionskybox = glm::perspective(glm::radians(camera.Zoom),
                                                      (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f,
                                                      100.0f);
        //glm::mat4 model = glm::rotate(model, glm::radians(r), glm::vec3(1.0, 1.0, 0.0));

        //天空盒渲染
        glDepthMask(GL_FALSE);


        skyboxShader.use();

        viewskybox = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        viewskybox = glm::rotate(viewskybox, (GLfloat) glfwGetTime() / 10,
                                 glm::vec3(0.0f, 0.0f, 1.0f));
        skyboxShader.setMat4("view", glm::value_ptr(viewskybox));
        skyboxShader.setMat4("projection", glm::value_ptr(projectionskybox));


        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glDepthMask(0xFF);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                                                (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f,
                                                200.0f);
        glm::mat4 view = camera.GetViewMatrix();

        // 将相机的位置传给MapManager，如果相机移动到另一个chunk，则更新cache
        int x, z;
//        mapManager.updateCacheMap(camera.Position);
        mapManager.update(camera.Position);
        // 获取cache的指针，以使用cache进行绘制
        Cache *mapCache = mapManager.getCache();
        std::tie(x, z) = mapManager.getCacheVertexCoord();

        // 遍历cache，根据cache里block的类型，调用cube的Draw函数进行绘制
        for (int cx = 0; cx < CHUNK_NUM; ++cx) {
            for (int cz = 0; cz < CHUNK_NUM; ++cz) {
                for (int i = 0; i < CHUNK_SIZE; i++) {
                    for (int k = 0; k < CHUNK_SIZE; k++) {
                        for (int j = 0; j < 64; j++) {
                            glm::vec3 pos = glm::vec3(cx * CHUNK_SIZE + x + i, j,
                                                      cz * CHUNK_SIZE + z + k);
                            if ((*mapCache)[cx][cz][i][j][k] != CubeType::NONE) {
                                cube.Draw(static_cast<CubeType>((*mapCache)[cx][cz][i][j][k]), pos,
                                          projection, view,
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
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

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
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
        camera.ProcessKeyboard(TAB, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        camera.remove();
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        camera.add();
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

//加载纹理盒
unsigned int loadCubemapsTexture(const std::vector<std::string> &fullpathVec) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrComponents;
    for (GLuint i = 0; i < fullpathVec.size(); ++i) {
        unsigned char *data = stbi_load(fullpathVec[i].c_str(), &width, &height, &nrComponents, 0);
        if (data) {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format,
                         GL_UNSIGNED_BYTE, data);
            printf("%d %d %d \n", width, height, nrComponents);
            stbi_image_free(data);
        } else {
            std::cout << "纹理加载失败，路径是:" << fullpathVec[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


    return textureID;
}
