#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "stb_image.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "TextureAtlas.h"
#include "noise.h"
#include "db/MCdb.h"
#include "db/Hash.h"
#include "model.h"
#include "Cube.h"
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
Camera camera(glm::vec3(0.0f, 15.0f, 3.0f));

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

    MCdb db(MCdb::Type::SQLITE);
    db.initDB("MineCraft", "localhost", "root", "233");

    //Model firTree("C:/Users/13353/Desktop/code/CG/Project/Project/obj/Wooden-Watch-Tower/wooden watch tower2.fbx");
    Model firTree("C:/Users/13353/Desktop/code/CG/Project/Project/obj/cat/Cat.obj");

    Shader modelShader("shaders/model.vs", "shaders/model.fs");
    //Shader modelShader("1.model_loading.vs", "1.model_loading.fs");
    Perlin noise;
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
        model_ = glm::translate(model_, glm::vec3(0.0f, 10.0f, 0.0f)); // translate it down so it's at the center of the scene
        model_ = glm::scale(model_, glm::vec3(0.01f, 0.01f, 0.01f));	// it's a bit too big for our scene, so scale it down
		model_ = glm::rotate(model_, -1.57f, glm::vec3(1, 0, 0));
		modelShader.setMat4("model", model_);
        firTree.Draw(modelShader);

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
					glm::vec3 pos = glm::vec3(stoi(row[1]), stoi(row[2]), stoi(row[3]));
                    if (stoi(row[4]) == 0) {
						cube.Draw("soil", pos, projection, view);
                    } else if (stoi(row[4]) == 1) {
						glm::mat4 view = camera.GetViewMatrix();
						cube.Draw("grass", pos, projection, view);
					}
					else if (stoi(row[4]) == 2) {
						cube.Draw("highGrass", pos, projection, view);
					}
					else if (stoi(row[4]) == 3) {
						cube.Draw("flower_1", pos, projection, view);
					}
					else if (stoi(row[4]) == 4) {
						cube.Draw("flower_2", pos, projection, view);
					}
					else if (stoi(row[4]) == 5) {
						cube.Draw("flower_3", pos, projection, view);
					}
					else if (stoi(row[4]) == 6) {
						cube.Draw("flower_4", pos, projection, view);
					}
					else if (stoi(row[4]) == 7) {
						cube.Draw("flower_5", pos, projection, view);
					}
					else if (stoi(row[4]) == 8) {
						cube.Draw("flower_6", pos, projection, view);
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
						int r = rand() % 1000 + 1;
						if (r % 7 == 0) {
							string tmpquery =
								"(" + to_string(chunkshouldshow[s]) + ","
								+ to_string(i) + "," + to_string(t+1) + "," + to_string(k)
								+ "," + to_string(int(2)) + ")";
							querys.push_back(tmpquery);
						}
						else if (r % 16 == 0) {
							string tmpquery =
								"(" + to_string(chunkshouldshow[s]) + ","
								+ to_string(i) + "," + to_string(t + 1) + "," + to_string(k)
								+ "," + to_string(int(3)) + ")";
							querys.push_back(tmpquery);
						}
						else if (r % 20 == 0) {
							string tmpquery =
								"(" + to_string(chunkshouldshow[s]) + ","
								+ to_string(i) + "," + to_string(t + 1) + "," + to_string(k)
								+ "," + to_string(int(4)) + ")";
							querys.push_back(tmpquery);
						}
						else if (r % 20 == 0) {
							string tmpquery =
								"(" + to_string(chunkshouldshow[s]) + ","
								+ to_string(i) + "," + to_string(t + 1) + "," + to_string(k)
								+ "," + to_string(int(5)) + ")";
							querys.push_back(tmpquery);
						}
						else if (r % 20 == 0) {
							string tmpquery =
								"(" + to_string(chunkshouldshow[s]) + ","
								+ to_string(i) + "," + to_string(t + 1) + "," + to_string(k)
								+ "," + to_string(int(6)) + ")";
							querys.push_back(tmpquery);
						}
						else if (r % 24 == 0) {
							string tmpquery =
								"(" + to_string(chunkshouldshow[s]) + ","
								+ to_string(i) + "," + to_string(t + 1) + "," + to_string(k)
								+ "," + to_string(int(7)) + ")";
							querys.push_back(tmpquery);
						}
						else if (r % 24 == 0) {
							string tmpquery =
								"(" + to_string(chunkshouldshow[s]) + ","
								+ to_string(i) + "," + to_string(t + 1) + "," + to_string(k)
								+ "," + to_string(int(8)) + ")";
							querys.push_back(tmpquery);
						}
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
