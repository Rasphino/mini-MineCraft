#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <iostream>
#include <assimp/scene.h>


#include "stb_image.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "TextureAtlas.h"
#include "noise.h"
#include "db/MCdb.h"
#include "db/Hash.h"
#include <string>
#include <vector>


using namespace std;
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
// camera
Camera camera(glm::vec3(256.0f, 20.0f, 256.0f));
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

    const float size = 1 / 16.0;
    VertexData grassCube[] = {
            //后面
            {{0.0f, 0.0f, 0.0f},
                    {size,     15 * size}},
            {{1.0f, 0.0f, 0.0f},
                    {2 * size, 15 * size}},
            {{1.0f, 1.0f, 0.0f},
                    {2 * size, 1.0f}},
            {{1.0f, 1.0f, 0.0f},
                    {2 * size, 1.0f}},
            {{0.0f, 1.0f, 0.0f},
                    {size,     1.0f}},
            {{0.0f, 0.0f, 0.0f},
                    {size,     15 * size}},

            //前面
            {{0.0f, 0.0f, 1.0f},
                    {size,     15 * size}},
            {{1.0f, 0.0f, 1.0f},
                    {2 * size, 15 * size}},
            {{1.0f, 1.0f, 1.0f},
                    {2 * size, 1.0f}},
            {{1.0f, 1.0f, 1.0f},
                    {2 * size, 1.0f}},
            {{0.0f, 1.0f, 1.0f},
                    {size,     1.0f}},
            {{0.0f, 0.0f, 1.0f},
                    {size,     15 * size}},

            //左面
            {{0.0f, 0.0f, 0.0f},
                    {size,     15 * size}},
            {{0.0f, 0.0f, 1.0f},
                    {2 * size, 15 * size}},
            {{0.0f, 1.0f, 1.0f},
                    {2 * size, 1.0f}},
            {{0.0f, 1.0f, 1.0f},
                    {2 * size, 1.0f}},
            {{0.0f, 1.0f, 0.0f},
                    {size,     1.0f}},
            {{0.0f, 0.0f, 0.0f},
                    {size,     15 * size}},

            //右面
            {{1.0f, 0.0f, 0.0f},
                    {size,     15 * size}},
            {{1.0f, 0.0f, 1.0f},
                    {2 * size, 15 * size}},
            {{1.0f, 1.0f, 1.0f},
                    {2 * size, 1.0f}},
            {{1.0f, 1.0f, 1.0f},
                    {2 * size, 1.0f}},
            {{1.0f, 1.0f, 0.0f},
                    {size,     1.0f}},
            {{1.0f, 0.0f, 0.0f},
                    {size,     15 * size}},

            //下面
            {{0.0f, 0.0f, 0.0f},
                    {2 * size, 15 * size}},
            {{1.0f, 0.0f, 0.0f},
                    {3 * size, 15 * size}},
            {{1.0f, 0.0f, 1.0f},
                    {3 * size, 1.0f}},
            {{1.0f, 0.0f, 1.0f},
                    {3 * size, 1.0f}},
            {{0.0f, 0.0f, 1.0f},
                    {2 * size, 1.0f}},
            {{0.0f, 0.0f, 0.0f},
                    {2 * size, 15 * size}},

            //上面
            {{0.0f, 1.0f, 0.0f},
                    {0.0f,     1.0f}},
            {{1.0f, 1.0f, 0.0f},
                    {size,     1.0f}},
            {{1.0f, 1.0f, 1.0f},
                    {size,     15 * size}},
            {{1.0f, 1.0f, 1.0f},
                    {size,     15 * size}},
            {{0.0f, 1.0f, 1.0f},
                    {0.0f,     15 * size}},
            {{0.0f, 1.0f, 0.0f},
                    {0.0f,     1.0f}},
    };

    VertexData soilCube[] = {
            //后面
            {{0.0f, 0.0f, 0.0f},
                    {size,     15 * size}},
            {{1.0f, 0.0f, 0.0f},
                    {2 * size, 15 * size}},
            {{1.0f, 1.0f, 0.0f},
                    {2 * size, 1.0f}},
            {{1.0f, 1.0f, 0.0f},
                    {2 * size, 1.0f}},
            {{0.0f, 1.0f, 0.0f},
                    {size,     1.0f}},
            {{0.0f, 0.0f, 0.0f},
                    {size,     15 * size}},

            //前面
            {{0.0f, 0.0f, 1.0f},
                    {size,     15 * size}},
            {{1.0f, 0.0f, 1.0f},
                    {2 * size, 15 * size}},
            {{1.0f, 1.0f, 1.0f},
                    {2 * size, 1.0f}},
            {{1.0f, 1.0f, 1.0f},
                    {2 * size, 1.0f}},
            {{0.0f, 1.0f, 1.0f},
                    {size,     1.0f}},
            {{0.0f, 0.0f, 1.0f},
                    {size,     15 * size}},

            //左面
            {{0.0f, 0.0f, 0.0f},
                    {size,     15 * size}},
            {{0.0f, 0.0f, 1.0f},
                    {2 * size, 15 * size}},
            {{0.0f, 1.0f, 1.0f},
                    {2 * size, 1.0f}},
            {{0.0f, 1.0f, 1.0f},
                    {2 * size, 1.0f}},
            {{0.0f, 1.0f, 0.0f},
                    {size,     1.0f}},
            {{0.0f, 0.0f, 0.0f},
                    {size,     15 * size}},

            //右面
            {{1.0f, 0.0f, 0.0f},
                    {size,     15 * size}},
            {{1.0f, 0.0f, 1.0f},
                    {2 * size, 15 * size}},
            {{1.0f, 1.0f, 1.0f},
                    {2 * size, 1.0f}},
            {{1.0f, 1.0f, 1.0f},
                    {2 * size, 1.0f}},
            {{1.0f, 1.0f, 0.0f},
                    {size,     1.0f}},
            {{1.0f, 0.0f, 0.0f},
                    {size,     15 * size}},

            //下面
            {{0.0f, 0.0f, 0.0f},
                    {2 * size, 15 * size}},
            {{1.0f, 0.0f, 0.0f},
                    {3 * size, 15 * size}},
            {{1.0f, 0.0f, 1.0f},
                    {3 * size, 1.0f}},
            {{1.0f, 0.0f, 1.0f},
                    {3 * size, 1.0f}},
            {{0.0f, 0.0f, 1.0f},
                    {2 * size, 1.0f}},
            {{0.0f, 0.0f, 0.0f},
                    {2 * size, 15 * size}},

            //上面
            {{0.0f, 1.0f, 0.0f},
                    {0.0f,     1.0f}},
            {{1.0f, 1.0f, 0.0f},
                    {size,     1.0f}},
            {{1.0f, 1.0f, 1.0f},
                    {size,     15 * size}},
            {{1.0f, 1.0f, 1.0f},
                    {size,     15 * size}},
            {{0.0f, 1.0f, 1.0f},
                    {0.0f,     15 * size}},
            {{0.0f, 1.0f, 0.0f},
                    {0.0f,     1.0f}},
    };

//    VertexData soilCube[] = {
//            //后面
//            {{-0.5f, -0.5f, -0.5f},
//                    {2 * size, 15 * size}},
//            {{0.5f,  -0.5f, -0.5f},
//                    {3 * size, 15 * size}},
//            {{0.5f,  0.5f,  -0.5f},
//                    {3 * size, 1.0f}},
//            {{0.5f,  0.5f,  -0.5f},
//                    {3 * size, 1.0f}},
//            {{-0.5f, 0.5f,  -0.5f},
//                    {2 * size, 1.0f}},
//            {{-0.5f, -0.5f, -0.5f},
//                    {2 * size, 15 * size}},
//
//            //前面
//            {{-0.5f, -0.5f, 0.5f},
//                    {2 * size, 15 * size}},
//            {{0.5f,  -0.5f, 0.5f},
//                    {3 * size, 15 * size}},
//            {{0.5f,  0.5f,  0.5f},
//                    {3 * size, 1.0f}},
//            {{0.5f,  0.5f,  0.5f},
//                    {3 * size, 1.0f}},
//            {{-0.5f, 0.5f,  0.5f},
//                    {2 * size, 1.0f}},
//            {{-0.5f, -0.5f, 0.5f},
//                    {2 * size, 15 * size}},
//            //左面
//            {{-0.5f, -0.5f, -0.5f},
//                    {2 * size, 15 * size}},
//            {{-0.5f, -0.5f, 0.5f},
//                    {3 * size, 15 * size}},
//            {{-0.5f, 0.5f,  0.5f},
//                    {3 * size, 1.0f}},
//            {{-0.5f, 0.5f,  0.5f},
//                    {3 * size, 1.0f}},
//            {{-0.5f, 0.5f,  -0.5f},
//                    {2 * size, 1.0f}},
//            {{-0.5f, -0.5f, -0.5f},
//                    {2 * size, 15 * size}},
//            //右面
//            {{0.5f,  -0.5f, -0.5f},
//                    {2 * size, 15 * size}},
//            {{0.5f,  -0.5f, 0.5f},
//                    {3 * size, 15 * size}},
//            {{0.5f,  0.5f,  0.5f},
//                    {3 * size, 1.0f}},
//            {{0.5f,  0.5f,  0.5f},
//                    {3 * size, 1.0f}},
//            {{0.5f,  0.5f,  -0.5f},
//                    {2 * size, 1.0f}},
//            {{0.5f,  -0.5f, -0.5f},
//                    {2 * size, 15 * size}},
//            //下面
//            {{-0.5f, -0.5f, -0.5f},
//                    {2 * size, 15 * size}},
//            {{0.5f,  -0.5f, -0.5f},
//                    {3 * size, 15 * size}},
//            {{0.5f,  -0.5f, 0.5f},
//                    {3 * size, 1.0f}},
//            {{0.5f,  -0.5f, 0.5f},
//                    {3 * size, 1.0f}},
//            {{-0.5f, -0.5f, 0.5f},
//                    {2 * size, 1.0f}},
//            {{-0.5f, -0.5f, -0.5f},
//                    {2 * size, 15 * size}},
//
//            //上面
//            {{-0.5f, 0.5f,  -0.5f},
//                    {2 * size, 15 * size}},
//            {{0.5f,  0.5f,  -0.5f},
//                    {3 * size, 15 * size}},
//            {{0.5f,  0.5f,  0.5f},
//                    {3 * size, 1.0f}},
//            {{0.5f,  0.5f,  0.5f},
//                    {3 * size, 1.0f}},
//            {{-0.5f, 0.5f,  0.5f},
//                    {2 * size, 1.0f}},
//            {{-0.5f, 0.5f,  -0.5f},
//                    {2 * size, 15 * size}},
//    };

    std::string file = "textures/DefaultPack.png";
    TextureAtlas tex(file);
    Shader grassShader("shaders/grass.vs", "shaders/grass.fs");
    unsigned int VBO_grass, VAO_grass;
    glGenVertexArrays(1, &VAO_grass);
    glGenBuffers(1, &VBO_grass);
    glBindVertexArray(VAO_grass);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_grass);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grassCube), grassCube, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *) 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                          (void *) (sizeof(grassCube[0].position)));

    Shader soilShader("shaders/soil.vs", "shaders/soil.fs");
    unsigned int VAO_soil, VBO_soil;
    glGenVertexArrays(1, &VAO_soil);
    glGenBuffers(1, &VBO_soil);
    glBindVertexArray(VAO_soil);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_soil);
    glBufferData(GL_ARRAY_BUFFER, sizeof(soilCube), soilCube, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *) 0);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                          (void *) (sizeof(soilCube[0].position)));

    MCdb db(MCdb::Type::SQLITE);
    db.initDB("MineCraft", "localhost", "root", "233");

    Perlin noise;
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        tex.TexActivate();
        tex.BindTexture();
        camera.ProcessJump();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                                                (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f,
                                                100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        //用hash来判断chunk id
        int chunkid = hash_getchunkid(camera.Position[0], camera.Position[2]);
//        cout << "chunkid" << chunkid <<" " << "isvisited:" << Chunkvisited[chunkid]<< endl;
        int chunkshouldshow[9] = {chunkid, chunkid - 1, chunkid + 1,
                                  chunkid + 32, chunkid + 32, chunkid + 33,
                                  chunkid - 31, chunkid - 32, chunkid - 33};
//        int chunshouldshow[]
        for (int s = 0; s < 9; s++) {
            if (Chunkvisited[chunkshouldshow[s]]) {
                string select_query;
                if (chunkshouldshow[s] >= 0)
                    select_query = "select * from block" + to_string(abs(chunkshouldshow[s]));
                Records result;
                db.execSQL(select_query, result);
                for (const auto &row : result) {
                    if (row.empty()) break;

                    // mysql_num_fields()返回结果集中的字段数
                    if (stoi(row[4]) == 0) {
                        glBindVertexArray(VAO_soil);
                        glBindBuffer(GL_ARRAY_BUFFER, VBO_soil);
                        glEnableVertexAttribArray(2);
                        glEnableVertexAttribArray(3);
                        soilShader.use();
                        glUniform1i(glGetUniformLocation(soilShader.ID, "soil"), 0);

                        soilShader.setMat4("projection", projection);
                        soilShader.setMat4("view", view);

                        glm::vec3 pos = glm::vec3(stoi(row[1]), stoi(row[2]), stoi(row[3]));
                        glm::mat4 model = glm::mat4(1.0f);
                        model = glm::translate(model, pos);
                        soilShader.setMat4("model", model);
                        glDrawArrays(GL_TRIANGLES, 0, 36);
                    } else if (stoi(row[4]) == 1) {
                        glBindVertexArray(VAO_grass);
                        glBindBuffer(GL_ARRAY_BUFFER, VBO_grass);
                        glEnableVertexAttribArray(0);
                        glEnableVertexAttribArray(1);
                        grassShader.use();
                        grassShader.setMat4("projection", projection);
                        glm::mat4 view = camera.GetViewMatrix();
                        grassShader.setMat4("view", view);
                        glUniform1i(glGetUniformLocation(grassShader.ID, "grass"), 0);

                        soilShader.setMat4("projection", projection);
                        soilShader.setMat4("view", view);
                        glm::vec3 pos = glm::vec3(stoi(row[1]), stoi(row[2]), stoi(row[3]));
                        glm::mat4 model = glm::mat4(1.0f);
                        model = glm::translate(model, pos);
                        grassShader.setMat4("model", model);
                        glDrawArrays(GL_TRIANGLES, 0, 36);
                    }
                }
            } else {
                string create_query =
                        "create table if not exists block" + to_string(abs(chunkshouldshow[s])) +
                        "("
                        "    chunkid int not null,"
                        "    x int not null,"
                        "    y int not null,"
                        "    z int not null,"
                        "    chunkType int not null,"
                        "   primary key(x, y, z)"
                        ");";
                db.execSQL(create_query);
                int zpos = (chunkshouldshow[s] / 32) * 16;
                int xpos = (chunkshouldshow[s] - 2 * zpos) * 16;
                vector<string> querys;
                for (int i = xpos; i < xpos + 16; i++) {
                    for (int k = zpos; k < zpos + 16; k++) {
                        int j = (int) ((noise.PerlinNoise(i * 0.1, k * 0.1) + 1) * 10);
                        int t = 0;
                        for (t = 0; t < j - 1; t++) {
                            string tmpquery =
                                    "(" + to_string(chunkshouldshow[s]) + ","
                                    + to_string(i) + "," + to_string(t) + "," + to_string(k)
                                    + "," + to_string(int(0)) + ")";
                            querys.push_back(tmpquery);
                        }
                        string tmpquery =
                                "(" + to_string(chunkshouldshow[s]) + ","
                                + to_string(i) + "," + to_string(t) + "," + to_string(k)
                                + "," + to_string(int(1)) + ")";
                        querys.push_back(tmpquery);
                    }
                }
                string insertQuery =
                        "replace into block" + to_string(abs(chunkshouldshow[s])) + " values ";
                int i = 0;
                for (i = 0; i < querys.size() - 1; i++) {
                    insertQuery += querys[i] + ",";
                }
                insertQuery += querys[i] + ";";
                db.execSQL(insertQuery);
                Chunkvisited[chunkshouldshow[s]] = 1;
            }
        }
        glUseProgram(0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
//    string delete_table =
//    "drop table MineCraft.block1;";
//    db.execSQL(delete_table);

    glDeleteVertexArrays(1, &VAO_grass);
    glDeleteBuffers(1, &VBO_grass);
    glDeleteVertexArrays(1, &VAO_soil);
    glDeleteBuffers(1, &VBO_soil);


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
