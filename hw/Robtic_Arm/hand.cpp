// This is the main project file for VC++ application project 
// generated using an Application Wizard.

#define GLUT_DISABLE_ATEXIT_HACK

#define PARTSNUM  32

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "./freeglut/glew.h"
#include "./freeglut/glut.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>


static int draw_mode=1;
static int action=0;
static GLuint inner_mode = GL_SMOOTH;
static GLuint tex_state = GL_TRUE;
static int state_chang;

GLuint VAO;
GLuint UBO;
glm::mat4 Projection;
glm::mat4 View ;
glm::mat4 Models[PARTSNUM];

glm::mat4 Rotatation[PARTSNUM];
glm::mat4 Translation[PARTSNUM];

GLuint program;

float anklea=0,		anklex=0,		ankley=0,		anklez=1;		//手肘
float plama,		plamx=0,		plamy=0,	plamz=0;			//手掌 (靜止)
float anklef1a[2],	anklef1x[2],	anklef1y[2],	anklef1z[2];	//拇指
float anklef2a[3],	anklef2x[3],	anklef2y[3],	anklef2z[3];	//食指
float anklef3a[3],	anklef3x[3],	anklef3y[3],	anklef3z[3];	//中指
float anklef4a[3],	anklef4x[3],	anklef4y[3],	anklef4z[3];	//無名指
float anklef5a[3],	anklef5x[3],	anklef5y[3],	anklef5z[3];	//小指
//	  ^^^轉動量		^^^轉動軸x		^^^轉動軸y		^^^轉動軸z

std::vector<glm::vec3> solid = {
	glm::vec3(7.0, 30.0, 30.0),		//上臂
	glm::vec3(5.0, 30.0, 30.0),		//肘關節
	glm::vec3(6.0, 30.0, 30.0),		//下臂
	glm::vec3(3.0, 30.0, 30.0),		//掌關節
	glm::vec3(8, 30.0, 30.0),		//掌
	glm::vec3(1.5, 30.0, 30.0),		//食指
	glm::vec3(2.5, 30.0, 30.0),		//食指節3
	glm::vec3(1.0,30.0,30.0),		//食指中關節
	glm::vec3(2.0,30.0,30.0),		//食指節2
	glm::vec3(1.0,30.0,30.0),		//食指上關節
	glm::vec3(1.7, 30.0, 30.0),		//食指節3
};

std::vector<glm::vec3> scales = {
	glm::vec3(4,1,1),			//上臂
	glm::vec3(1,1,1),			//肘關節
	glm::vec3(4,1,1),			//下臂
	glm::vec3(1, 1, 1),			//掌關節
	glm::vec3(1, 1, 0.5),		//掌
	glm::vec3(1, 1, 1),			//食指
	glm::vec3(1.2, 0.6, 1),		//食指節3
	glm::vec3(1.0,1.0,1.0),		//食指中關節
	glm::vec3(1.0,0.6,1),		//食指節2
	glm::vec3(1.0,1,1.0),		//食指上關節
	glm::vec3(1.0, 0.6, 1.0),	//食指節3
};

std::vector<glm::vec3> colors = {
	glm::vec3(255,177,98),		//上臂
	glm::vec3(219,156,53),		//肘關節
	glm::vec3(255,177,98),		//下臂
	glm::vec3(219,156,53),		//掌關節
	glm::vec3(255,102,125),		//掌
	glm::vec3(0,0,0),			//食指
	glm::vec3(255,177,98),		//食指節3
	glm::vec3(0,0,0),			//食指中關節3
	glm::vec3(89,197,255),		//食指節
	glm::vec3(0,0,0),			//食指上關節2
	glm::vec3(0,177,98),		//食指節3
};

int state=1;

static int WinWidth = 600;
static int WinHeight = 600;

GLuint shaderProgram;

// Define the model, view, and projection matrices
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

// Function to create the shader program
void createShaderProgram() {
	// Vertex shader source code
	const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
		uniform vec3 scale; 

        void main() {
			vec4 scaledPosition = vec4(aPos * scale, 1.0);
			gl_Position = projection * view * model * scaledPosition;
        }
    )";

	// Fragment shader source code
	const char* fragmentShaderSource = R"(
        #version 330 core
		uniform vec3 objectColor;
        out vec4 FragColor;

        void main() {
            FragColor = vec4(objectColor, 1.0);
        }
    )";

	// Compile the vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Compile the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Create the shader program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Delete the shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}


void standby() {
	state = 1;
	anklea = 120, anklex = 0, ankley = 0, anklez = 1;  //手掌
	plama = 0, plamx = 0, plamy = 0, plamz = 0;  //手掌

	//-----------------------------------------	
	anklef1a[0] = 15;	//拇指接掌
	anklef1x[0] = 0;
	anklef1y[0] = 0;
	anklef1z[0] = 1;

	anklef1a[1] = -10;	//拇指1
	anklef1x[1] = 0;
	anklef1y[1] = 0;
	anklef1z[1] = 1;

	//-----------------------------------------	
	anklef2a[0] = 15;		//食指接掌
	anklef2x[0] = 0;
	anklef2y[0] = 0;
	anklef2z[0] = 1;

	anklef2a[1] = -10;	//食指1
	anklef2x[1] = 0;
	anklef2y[1] = 0;
	anklef2z[1] = 1;

	anklef2a[2] = -10;	//食指2
	anklef2x[2] = 0;
	anklef2y[2] = 0;
	anklef2z[2] = 1;
	//-----------------------------------------	
	anklef3a[0] = 15;	//中指接掌
	anklef3x[0] = 0;
	anklef3y[0] = 0;
	anklef3z[0] = 1;

	anklef3a[1] = -10;	//中指1
	anklef3x[1] = 0;
	anklef3y[1] = 0;
	anklef3z[1] = 1;

	anklef3a[2] = -10;	//中指2
	anklef3x[2] = 0;
	anklef3y[2] = 0;
	anklef3z[2] = 1;

	//-----------------------------------------	
	anklef4a[0] = 15;	//無名指接掌
	anklef4x[0] = 0;
	anklef4y[0] = 0;
	anklef4z[0] = 1;

	anklef4a[1] = -10;	//無名指1
	anklef4x[1] = 0;
	anklef4y[1] = 0;
	anklef4z[1] = 1;

	anklef4a[2] = -10;	//無名指2
	anklef4x[2] = 0;
	anklef4y[2] = 0;
	anklef4z[2] = 1;

	//-----------------------------------------	
	anklef5a[0] = 15;	//小指接掌
	anklef5x[0] = 0;
	anklef5y[0] = 0;
	anklef5z[0] = 1;

	anklef5a[1] = -10;	//小指1
	anklef5x[1] = 0;
	anklef5y[1] = 0;
	anklef5z[1] = 1;

	anklef5a[2] = -10;	//小指2
	anklef5x[2] = 0;
	anklef5y[2] = 0;
	anklef5z[2] = 1;
}

void updateFingers(glm::vec3 offset ,int index ,int n , float anklefa[] , float anklefx[] , float anklefy[] , float anklefz[])
{
	Translation[index] = glm::translate(Translation[index], offset);
	Rotatation[index] = glm::rotate(Rotatation[index], glm::radians(anklefa[0]), glm::vec3(anklefx[0], anklefy[0], anklefz[0]));
	Models[index] = Models[4] * Translation[index] * Rotatation[index]; //食指

	Translation[index + 1] = glm::translate(Translation[index + 1], glm::vec3(-4.0, 0.0, 0.0));
	Models[index + 1] = Models[index] * Translation[index + 1] * Rotatation[index + 1]; //食指節3

	Translation[index + 2] = glm::translate(Translation[index +2], glm::vec3(-4.0, 0.0, 0.0));
	Rotatation[index + 2] = glm::rotate(Rotatation[index + 2], glm::radians(anklefa[1]), glm::vec3(anklefx[1], anklefy[1], anklefz[1]));
	Models[index + 2] = Models[index+1] * Translation[index + 2] * Rotatation[index + 2 ]; //食指中關節

	Translation[index + 3] = glm::translate(Translation[index + 3], glm::vec3(-3.0, 0.0, 0.0));
	Models[index + 3] = Models[index+2] * Translation[index + 3] * Rotatation[index + 3]; //食指節2

	if (n == 2)return;

	Translation[index + 4] = glm::translate(Translation[index + 4], glm::vec3(-3.0, 0.0, 0.0));
	Rotatation[index + 4] = glm::rotate(Rotatation[index + 4], glm::radians(anklefa[2]), glm::vec3(anklefx[2], anklefy[2], anklefz[2]));
	Models[index + 4] = Models[index+3] * Translation[index + 4] * Rotatation[index + 4]; //食指上關節

	Translation[index + 5] = glm::translate(Translation[index + 5], glm::vec3(-2.5, 0.0, 0.0));
	Models[index + 5] = Models[index + 4] * Translation[index + 5] * Rotatation[index + 5]; //食指節3
}

void updateModels()
{
	for (int i = 0; i < PARTSNUM; i++) {
		Models[i] = glm::mat4(1.0f);
		Rotatation[i] = glm::mat4(1.0f);
		Translation[i] = glm::mat4(1.0f);
	}
	Translation[0] = glm::translate(Translation[0], glm::vec3(55.0, 0.0, -200.0));
	Rotatation[0] = glm::rotate(Rotatation[0], glm::radians(180.f), glm::vec3(1, 0, 0));

	Models[0] = Translation[0] * Rotatation[0];//上臂

	Translation[1] = glm::translate(Translation[1], glm::vec3(anklex - 25, 0.0, 0.0));
	Rotatation[1] = glm::rotate(Rotatation[1], glm::radians(anklea), glm::vec3(anklex, ankley, anklez));
	Models[1] = Models[0]*Translation[1] * Rotatation[1]; //肘關節

	Translation[2] = glm::translate(Translation[2], glm::vec3(-22.0, 0.0, 0.0));
	Models[2] = Models[1]*Translation[2] * Rotatation[2]; //下臂

	Translation[3] = glm::translate(Translation[3], glm::vec3(-22.0, 0.0, 0.0));
	Models[3] = Models[2]*Translation[3] * Rotatation[3]; //掌關節

	Translation[4] = glm::translate(Translation[4], glm::vec3(-10.0, 0.0, 3));
	Models[4] = Models[3]*Translation[4] * Rotatation[4]; //掌

	updateFingers(glm::vec3(-10.0, -10.0, 10.0), 5, 2, anklef1a, anklef1x, anklef1y, anklef1z);	//拇指
	updateFingers(glm::vec3(-10.0, -5.0, 3.0), 9, 3, anklef2a, anklef2x, anklef2y, anklef2z);	//食指
	updateFingers(glm::vec3(-10.0, 0.0, 3.0), 15, 3, anklef3a, anklef3x, anklef3y, anklef3z);	//中指
	updateFingers(glm::vec3(-10.0, 5.0, 3.0), 21, 3, anklef4a, anklef4x, anklef4y, anklef4z);	//無名指
	updateFingers(glm::vec3(-10.0, 10.0, 3.0), 27, 3, anklef5a, anklef5x, anklef5y, anklef5z);	//小指
}

// Function to render the scene
void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glShadeModel(inner_mode);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Use the shader program
	glUseProgram(shaderProgram);

	// Set the model, view, and projection matrices
	GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	GLint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	GLuint scaleLoc = glGetUniformLocation(shaderProgram, "scale");
	glUniform3f(scaleLoc, 4, 1, 1); 
	GLuint objectColorLoc = glGetUniformLocation(shaderProgram, "objectColor");
	glUniform3f(objectColorLoc, 1, 0, 0);

	updateModels();

	
	for (int i = 0 , j = 0; i < PARTSNUM; i++ , j ++)
	{
		if (i == 9 || i == 15 || i == 21 || i == 27 )
		{
			j = 5;
		}
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(Models[i]));
		glUniform3f(scaleLoc, scales[j][0], scales[j][1], scales[j][2]);
		//glUniformMatrix4fv(objectColorLoc, 1, GL_FALSE, glm::value_ptr(colors[i]));
		glUniform3f(objectColorLoc, colors[j][0]*(float)1/255, colors[j][1]*(float)1/255, colors[j][2]*(float)1/255);
		glutSolidSphere(solid[j][0],solid[j][1],solid[j][2]);
	}


	// Swap buffers
	glutSwapBuffers();
}

void init(void)
{
	GLfloat  ambientLight[] = { 1, 1, 1, 0.0f };
	GLfloat  diffuseLight[] = { 0, 0, 0, 1.0f };
	GLfloat  specular[] = { 0.8f, 0.8f, 0.8f, 1.0f};
	GLfloat  ambientLight2[] = { 0.9f, 0.1f, 0.1f, 1.0f };
	GLfloat  specular2[] = { 0.3f, 0.3f, 0.3f, 1.3f};
	GLfloat  specref[] =  { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat  pos[] =  { 0.0f, 20.0f, -100.0f, 1.0f };
	GLfloat  dir[] =  { 0.0f, 0.0f, -1.0f};

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);

	// Setup and enable light 0
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
	glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT1,GL_AMBIENT,ambientLight2);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,ambientLight2);
	glLightfv(GL_LIGHT1,GL_SPECULAR,specular2);
	glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,dir);
	glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,60);
	glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,100);
	glLightfv(GL_LIGHT1,GL_POSITION,pos);

	// Enable color tracking
	glEnable(GL_COLOR_MATERIAL);

	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	// All materials hereafter have full specular reflectivity with a high shine
	glMaterialfv(GL_FRONT, GL_SPECULAR,specref);
	glMateriali(GL_FRONT,GL_SHININESS,128);

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
}

void hook_finger(int n, float anklefa[] , float anklefx[] , float anklefy[] ,float anklefz[])
{
	for (int i = 0; i < n; i++)
	{
		if (anklefa[i] >= -90)
			anklefa[i]--;
		anklefx[i] = 0;
		anklefy[i] = 1;
		anklefz[i] = 0;
		if (n == 2)
		{
			anklefx[i] = -1;
			anklefz[i] = 1;
		}
	}
}

void straight_finger(int n, float anklefa[], float anklefx[], float anklefy[], float anklefz[])
{
	for (int i = 0; i < n; i++)
	{	
		anklefa[i] = 15;
		if (i > 0)
			anklefa[i] = -10;
		anklefx[i] = 0;
		anklefy[i] = 0;
		anklefz[i] = 1;
	}
}

void rock_pos(void)
{
	if(anklea>=0)
		anklea--;
	state=0;
	//-----------------------------------------	
	hook_finger(2, anklef1a, anklef1x, anklef1y, anklef1z); //拇指
	hook_finger(3, anklef2a, anklef2x, anklef2y, anklef2z);	//食指
	hook_finger(3, anklef3a, anklef3x, anklef3y, anklef3z);	//中指
	hook_finger(3, anklef4a, anklef4x, anklef4y, anklef4z);	//無名指
	hook_finger(3, anklef5a, anklef5x, anklef5y, anklef5z);	//小指
	glutPostRedisplay();
}

void paper_pos(void)
{
	if (anklea >= 0)
		anklea--;
	state = 0;

	straight_finger(2, anklef1a, anklef1x, anklef1y, anklef1z); //拇指
	straight_finger(3, anklef2a, anklef2x, anklef2y, anklef2z);	//食指
	straight_finger(3, anklef3a, anklef3x, anklef3y, anklef3z);	//中指
	straight_finger(3, anklef4a, anklef4x, anklef4y, anklef4z);	//無名指
	straight_finger(3, anklef5a, anklef5x, anklef5y, anklef5z);	//小指

	glutPostRedisplay();
}

void scissors_pos(void)
{
	if (anklea >= 0)
		anklea--;
	state = 0;

	hook_finger(2, anklef1a, anklef1x, anklef1y, anklef1z); //拇指
	straight_finger(3, anklef2a, anklef2x, anklef2y, anklef2z);	//食指
	straight_finger(3, anklef3a, anklef3x, anklef3y, anklef3z);	//中指
	hook_finger(3, anklef4a, anklef4x, anklef4y, anklef4z);	//無名指
	hook_finger(3, anklef5a, anklef5x, anklef5y, anklef5z);	//小指

	glutPostRedisplay();
}

void one(void)                 //1
{
	if(anklea>=60)
		anklea--;
	state=0;

	//-----------------------------------------	
	anklef2x[0]=0;    //食指接掌
	anklef2y[0]=0;
	anklef2z[0]=0; 

	anklef2x[1]=0;    //食指1
	anklef2y[1]=0;
	anklef2z[1]=0;

	anklef2x[2]=0;    //食指2
	anklef2y[2]=0;
	anklef2z[2]=0;

	glutPostRedisplay();
}

void keyin(unsigned char key, int x, int y)
{
	if(state!=1){
		action = 0;
		standby();
	}

	switch(key)
	{
	case 'z':
		action = 1;
		break;
	case 'x':
		action = 2;
		break;
	case 'c':
		action = 3;
		break;
	}
}
void timerFunction(int value)
{
	switch(action)
	{
	case 0:
		standby();
		break;
	case 1:
		scissors_pos();
		break;
	case 2:
		rock_pos();
		break;
	case 3:
		paper_pos();
		break;
	}
	glutPostRedisplay();
	glutTimerFunc(5, timerFunction, 1);
}
int main(int argc,char** argv)
{

	int main_menu,action_menu,count_menu;

	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	
	//glutInitWindowSize(1200,600);	//< Dual view
	glutInitWindowSize(WinWidth,WinHeight);	//< Single view

	glutInitWindowPosition(0,0);
	glutCreateWindow(argv[0]);

	glewExperimental = GL_TRUE; //置於glewInit()之前
	if (glewInit()) {
		exit(EXIT_FAILURE);
	}

	init();

	standby();
	
	createShaderProgram();

	// Set the initial model, view, and projection matrices
	model = glm::mat4(1.0f);
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, 50.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	projection = glm::perspective(45.f, 1.0f, 1.0f, 500.0f);

	glutKeyboardFunc(keyin);
	glutDisplayFunc(render);
	glutTimerFunc(5, timerFunction, 1);
	glutMainLoop();
	return 0;

}

