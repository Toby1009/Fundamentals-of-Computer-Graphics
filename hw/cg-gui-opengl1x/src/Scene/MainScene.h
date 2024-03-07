#pragma once

#include <array>
#include <string>
#include <map>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define PI 3.1415926f

namespace CG
{
	class MainScene
	{
	public:

		struct Pos {
			GLfloat x=0, y=0; 
		};
		MainScene();
		~MainScene();

		auto Initialize() -> bool;
		void Update(double dt);
		void Render();

		void OnResize(int width, int height);
		void OnKeyboard(int key, int action);

		void OnScroll(int offset);

		static void mouse_callback(GLFWwindow* window, double xoffset, double yoffset);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

		void OnMouse(Pos offset);

		void SetMode(int mode);

	private:
		GLenum mode = GL_FILL; // GL_FILL or GL_LINE
		const GLfloat ANGLE_Diff = 2*PI / 5; // cycle dovoded into five equal parts
		GLfloat bigAngle ; // init angle 90
		GLfloat smallAngle; //init angle 90
		GLfloat r;//small cycle
		GLfloat R; //big cycle
		Pos originPos;
		Pos smallOPos, bigOPos; //small cycle pos , big cycle pos
		GLfloat rotated = 0;
		

	};
}

