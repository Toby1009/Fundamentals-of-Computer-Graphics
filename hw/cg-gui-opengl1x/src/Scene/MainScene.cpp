#include <iostream>

#include "MainScene.h"



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
    }

    void MainScene::OnResize(int width, int height)
    {
        std::cout << "MainScene Resize: " << width << " " << height << std::endl;
    }

    void MainScene::OnKeyboard(int key, int action)
    {
        std::cout << "MainScene OnKeyboard: " << key << " " << action << std::endl;

        if (action == GLFW_RELEASE)
        {
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
            case GLFW_KEY_4:
                rotated += PI / 4;
                break;
            case GLFW_KEY_5:
                rotated -= PI / 4;
                break;
            }
        }
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