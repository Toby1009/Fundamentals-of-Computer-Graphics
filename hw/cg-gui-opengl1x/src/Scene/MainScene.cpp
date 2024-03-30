#include <iostream>

#include "MainScene.h"
#include "GL/GL.h"
#include "GL/GLU.h"
#include "freeglut/glut.h"

using namespace std;

GLuint			triangle_buffer;
GLuint			triangle_vao;
GLuint			program;


static GLfloat rotated = 0;
static GLfloat speed = 0;
static const GLfloat triangle_vertices[] =
{
	 0.0f, 0.5f, 0.0f, 1.0f,
	 0.4f,  0.0f, 0.0f, 1.0f,
	 -0.4f, 0.0f, 0.0f, 1.0f,
};


vector<glm::vec4>tv2;
glm::vec4 tv[3];
GLfloat co2[30];

const char* vsSource = R"glsl(
	#version 400

	layout (location = 1) in vec4 position;
    uniform mat4 model;
    uniform mat4 view;  
    uniform mat4 projection;
	out vec4 vs_color;

	void main()
	{
		gl_Position = projection*view*model*position ;
	}
)glsl";
const char* fsSource = R"glsl(
	#version 400

	out vec4 color;

	void main()
	{
		color = vec4(1.0,0.0,0.0,1.0);
	}
)glsl";


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
		//glColor3f(1, 0, 0);
	}

	MainScene::~MainScene()
	{
	}

	void  MainScene::setStar()
	{
		tv2.clear();
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

			tv2.push_back(glm::vec4(bigOPos.x, bigOPos.y, 0.0f, 1.0f));

			tv2.push_back(glm::vec4(smallOPos.x, smallOPos.y, 0.0f, 1.0f));

			tv2.push_back(glm::vec4(originPos.x, originPos.y, 0.0f, 1.0f));

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

	auto MainScene::Initialize() -> bool
	{

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, mode);

		glewInit();

		setStar();

		//Initialize shaders
		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vs, 1, &vsSource, NULL);
		glShaderSource(fs, 1, &fsSource, NULL);

		glCompileShader(vs);
		glCompileShader(fs);

		//Attach Shader to program
		program = glCreateProgram();
		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);

		///////////////////////////	
		glGenVertexArrays(1, &triangle_vao);
		glBindVertexArray(triangle_vao);
		glGenBuffers(1, &triangle_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, triangle_buffer);
		glBufferData(GL_ARRAY_BUFFER, tv2.size() * sizeof(glm::vec4), &tv2[0], GL_STREAM_DRAW);

		glPointSize(40.0f);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

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

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Update shaders' input variable
		///////////////////////////	
		glm::mat4 model = glm::mat4(1.0f);
		rotated += speed;
		model = glm::rotate(model, glm::radians(rotated), glm::vec3(0.0f, 0.0f, 1.0f));
		glUseProgram(program);
		glBindVertexArray(triangle_vao);
		GLint uniTrans = glGetUniformLocation(program, "model");
		glUniformMatrix4fv(uniTrans, 1, GL_FALSE, &model[0][0]);

		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)100 / (float)100, 0.1f, 100.0f);
		GLint uniPro = glGetUniformLocation(program, "projection");
		glUniformMatrix4fv(uniPro, 1, GL_FALSE, &projection[0][0]);


		glm::mat4 view;

		view = glm::lookAt(
			cameraPos,
			cameraPos + cameraFront,
			cameraUp
		);

		GLint uniView = glGetUniformLocation(program, "view");
		glUniformMatrix4fv(uniView, 1, GL_FALSE, &view[0][0]);



		glDrawArrays(GL_TRIANGLES, 0, tv2.size());
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
		case GLFW_KEY_2: // green
			glColor3f(0, 1, 0);
			break;
		case GLFW_KEY_3: // blue
			glColor3f(0, 0, 1);
			break;
		case GLFW_KEY_W:
			cameraPos += cameraSpeed * cameraFront;
			break;
		case GLFW_KEY_S:
			cameraPos -= cameraSpeed * cameraFront;
			break;
		case GLFW_KEY_A:
			cameraPos -= cameraSpeed * cameraRight;
			break;
		case GLFW_KEY_D:
			cameraPos += cameraSpeed * cameraRight;
			break;
		case GLFW_KEY_Q:
			cameraPos -= cameraSpeed * cameraUp;
			break;
		case GLFW_KEY_E:
			cameraPos += cameraSpeed * cameraUp;
			break;
		case GLFW_KEY_0:
			speed += 3;
			break;
		case GLFW_KEY_9:
			speed -= 3;
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