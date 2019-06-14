#ifndef OPENGL_TEST_CAMERA_H
#define OPENGL_TEST_CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "MapManager.h"

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    JUMP,
    CROUCH,
    TAB,
    DOWN
};

enum direction {
    front,
    back,
    goleft,
    goright,
    up,
    down
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 5.0f;
const float SENSITIVITY = 0.2f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    glm::vec3 WorldDown;
    glm::vec3 fix;
    // Euler Angles
    float Yaw;
    float Pitch;
    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    bool jump = false, crouch = false, stopJump = false;
    int jumpCounter = 0;
    int tab = 1;
    float tempy;
    float ground;
    bool stopMove = false;
    float fixn = 0.05;

    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
            //glm::vec3 down = glm::vec3(0.0f, -1.0f, 0.0f),
           float yaw = YAW, float pitch = PITCH)
            : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
              MouseSensitivity(SENSITIVITY), Zoom(ZOOM), fix(glm::vec3(0.2f, 0.0f, 0.2f)) {
        Position = position;
        WorldUp = up;
        //WorldDown = down;
        Yaw = yaw;
        Pitch = pitch;
        tempy = Position.y;
        updateCameraVectors();
    }
    /*
    // Constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ,
            //float downX, float downY, float downZ,
            float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
    MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        //WorldDown = glm::vec3(downX, downY, downZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    */
    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix() {
        return glm::lookAt(Position, Position + Front, Up);
    }

    float vel;

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
        float velocity = MovementSpeed * deltaTime;
        vel = velocity;
        switch (direction) {
            case TAB:
                tab = -tab;  //tab = 1 -> fly
//                std::cout << "tab: " << tab << std::endl;
                break;
            case FORWARD:
                Position += Front * velocity;
                //std::cout << "Front : " << Front.x << " " << Front.y << " " << Front.z << std::endl;
                break;
            case BACKWARD:
                Position -= Front * velocity;
                break;
            case LEFT:
                Position -= Right * velocity;
                break;
            case RIGHT:
                Position += Right * velocity;
                break;
            case DOWN :
                Position -= WorldUp * velocity;
                tempy = Position.y;
                break;
            case JUMP:
                std::cout << "jumpcounter: " << jumpCounter << std::endl;
                std::cout << stopJump << std::endl;
                if (tab == -1 && !stopJump) {
                    jump = true;
                } else if (tab == 1) {
                    Position += WorldUp * velocity;
                    jump = false;
                    jumpCounter = 0;
                } else {
                    jump = false;
                }
                break;
        }
    }

    void ProcessFall() {
        bool fall = checkCollide(down);
        if (tab == -1 && !jump && !fall) { //fall
            Position.y -= 1;
            stopJump = true;
        } else if (tab == -1 && !jump && fall) { //on ground
            Position.y = ground;
            stopJump = false;
        } else if (tab == -1 && jump) { //jump
//            std::cout << "in jump: " << jumpCounter << std::endl;
            Position.y += 0.4 * sin(jumpCounter++ * glm::radians(40.0f));
//            std::cout << Position.y << std::endl;
            if (jumpCounter % 5 == 0) {
                jumpCounter = 0;
                jump = false;
                stopJump = true;
            }
        }
    }

    bool checkCollide(direction dir) {
        glm::vec3 Next;
        /*glm::vec3 smallNext;*/
        MapManager nextMap(Position);
        bool collide = false;
        int x, z, cx, cz, ci, cj, ck;
        /*int sx, sz, scx, scz, sci, scj, sck;*/
        switch (dir) {
            case front:
                Next = Position + Front * vel;
//                smallNext = Position + Front * fixn;
//                        - fix;
                break;
            case back:
                Next = Position - Front * vel;
//                smallNext = Position - Front * fixn;
                //+ fix;
                break;
            case goleft:
                Next = Position - Right * vel;
//                smallNext = Position - Right * fixn;
                //- fix ;
                break;
            case goright:
                Next = Position + Right * vel;
//                smallNext = Position + Right * fixn;
                //+ fix;
                break;
            case up:
                Next = Position + WorldUp;
                break;
            case down:
                Next = Position;
        }
        nextMap.updateCacheMap(Next);
        Cache *mapCache = nextMap.getCache();
        std::tie(x, z) = nextMap.getCacheVertexCoord();
        cx = (Next[0] - x) / CHUNK_SIZE;
        cz = (Next[2] - z) / CHUNK_SIZE;
        ci = Next[0] - x - cx * CHUNK_SIZE;
        ck = Next[2] - z - cz * CHUNK_SIZE;
        cj = (int) Position[1] - 1;
        /*
        scx = (smallNext[0] - x) / CHUNK_SIZE;
        scz = (smallNext[2] - z) / CHUNK_SIZE;
        sci = smallNext[0] - x - scx * CHUNK_SIZE;
        sck = smallNext[2] - z - scz * CHUNK_SIZE;
        scj = (int) Position[1];*/

        if (dir == up) cj++;
        if (dir == down) cj--;

//        std::cout << vel << std::endl;
//        if(dir!=5) std::cout << dir <<  Position[0] << " " << Position[2] << " " << Next[0] << " " << Next[2] << std::endl;
        /*
         if(dir != up && dir != down){
             if((*mapCache)[scx][scz][sci][scj][sck] == CubeType::SOIL || (*mapCache)[scx][scz][sci][scj][sck] == CubeType::GRASS)
                 collide = true;
         }*/

        if ((*mapCache)[cx][cz][ci][cj][ck] == CubeType::SOIL ||
            (*mapCache)[cx][cz][ci][cj][ck] == CubeType::GRASS) {
            if (dir == down) ground = cj + 2.4;
            collide = true;
        }
        return collide;
    }


    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch) {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // Update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset) {
        if (Zoom >= 1.0f && Zoom <= 45.0f)
            Zoom -= yoffset;
        if (Zoom <= 1.0f)
            Zoom = 1.0f;
        if (Zoom >= 45.0f)
            Zoom = 45.0f;
    }

private:
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors() {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front,
                                          WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }
};

#endif //OPENGL_TEST_CAMERA_H
