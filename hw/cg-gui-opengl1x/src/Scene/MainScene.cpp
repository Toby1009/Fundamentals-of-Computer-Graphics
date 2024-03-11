#include <iostream>

#include "MainScene.h"

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
//glm::vec3 cameraSpeed = glm::vec3(0.1f, 0.0f, 0.1f);

const float cameraSpeed = 0.05;

float fov = 60.0f;

bool firstMouse = true;


float lastX = 0, lastY = 0;
double yaw = -90, pitch = 0;

namespace CG
{
    MainScene::MainScene()
    {
        glColor3f(1, 0, 0);
    }

    MainScene::~MainScene()
    {
    }

    auto MainScene::Initialize() -> bool
    {
        glEnable(GL_DEPTH_TEST);
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
        
        return true;
    }

    void MainScene::Update(double dt)
    {

    }

    void MainScene::OnScroll(int offset)
    {
       // fovy -= offset;
    }

    void MainScene::Render()
    {

        glClearColor(0.0, 0.0, 0.0, 1); // Black background
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK, mode);

        //init 
        bigAngle = PI / 2 + rotated; // init angle 90
        smallAngle = PI / 2 + ANGLE_Diff / 2 + rotated; //init angle 90
        r = 0.167;//small cycle
        R = 0.5; //big cycle
        originPos = { 0,0 };

        for (int i = 0; i < 10; i++)
        {

            bigOPos = { originPos.x + R * cos(bigAngle),originPos.y + R * sin(bigAngle) };
            smallOPos = { originPos.x + r * cos(smallAngle) , originPos.y + r * sin(smallAngle) };

            glBegin(GL_POLYGON);
            if (i % 2 == 0)
            {
                glVertex2f(bigOPos.x, bigOPos.y);
                glVertex2f(smallOPos.x, smallOPos.y);
                glVertex2f(originPos.x, originPos.y);
            }
            else
            {
                glVertex2f(originPos.x, originPos.y);
                glVertex2f(smallOPos.x, smallOPos.y);
                glVertex2f(bigOPos.x, bigOPos.y);
            }

            glEnd();
            if (i % 2 == 0)
            {
                bigAngle += ANGLE_Diff;
            }
            else
            {
                smallAngle += ANGLE_Diff;
            }
        }

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glViewport(0, 0, 100, 100);
        gluPerspective(fov, 1, 1, 10.0f);
        gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2]
            , cameraPos[0] + cameraFront[0], cameraPos[1] + cameraFront[1], cameraPos[2] + cameraFront[2],
            cameraUp[0], cameraUp[1], cameraUp[2]);
        

    }

    void MainScene::OnMouse(Pos offset)
    {
        if (firstMouse == true)
        {
            lastX = offset.x;
            lastY = offset.y;
            firstMouse = false;
            return;
        }

        float xoffset = offset.x - lastX;
        float yoffset = lastY - offset.y;
        lastX = offset.x;
        lastY = offset.y;


        float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction);
    }


    void MainScene::OnResize(int width, int height)
    {
        std::cout << "MainScene Resize: " << width << " " << height << std::endl;
    }
 
    

    void MainScene::OnKeyboard(int key, int action)
    {
        std::cout << "MainScene OnKeyboard: " << key << " " << action << std::endl;

        switch (key)
            {
            case GLFW_KEY_1: // Red
                glColor3f(1, 0, 0);
                break;
            case GLFW_KEY_2 : // green
                glColor3f(0, 1, 0);
                break;
            case GLFW_KEY_3: // blue
                glColor3f(0, 0, 1);
                break;
            case GLFW_KEY_W:
                cameraPos +=  cameraSpeed * cameraFront;
                break;
            case GLFW_KEY_S:
                cameraPos -= cameraSpeed * cameraFront;
                break;
            case GLFW_KEY_A:
                cameraPos += cameraSpeed * cameraRight;
                break;
            case GLFW_KEY_D:
                cameraPos -= cameraSpeed * cameraRight;
                break;
            case GLFW_KEY_Q:
                cameraPos -= cameraSpeed * cameraUp;
                break;
            case GLFW_KEY_E:
                cameraPos += cameraSpeed * cameraUp;
                break;

            }
       
    }

    void MainScene::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        fov -= (float)yoffset;
        if (fov < 1.0f)
            fov = 1.0f;
        if (fov > 45.0f)
            fov = 45.0f;
    }

    void MainScene::SetMode(int mode)
    {
        switch (mode)
        {
        case 0:
            this->mode = GL_FILL;
            break;
        case 1:
            this->mode = GL_LINE;
            break;
        default:
            this->mode = GL_FILL;
            break;
        }
    }
}