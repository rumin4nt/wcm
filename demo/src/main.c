//
//  main.c
//  wcm_demo
//
//  Created by vs on 3/21/18.
//  Copyright © 2018 vaporstack. All rights reserved.
//

#include <stdio.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <GLFW/glfw3.h>
#pragma clang diagnostic pop

GLFWwindow* window = NULL;

#include "wcm.h"

#define WIDTH 256
#define HEIGHT 256

static int window_w = WIDTH;
static int window_h = HEIGHT;
static double mouse_x = 0;
static double mouse_y = 0;

static void joystick_callback(int joy, int event)
{
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
}

static void window_pos_callback(GLFWwindow* window, int x, int y)
{
}

static void window_size_callback(GLFWwindow* window, int width, int height)
{
	window_w = width;
	window_h = height;

	wcm_resize(width, height);
}

// static void draw_joysticks(void);

static void scroll_callback(GLFWwindow* window, double x, double y)
{
}

static void mouse_button_callback(GLFWwindow* window, int button, int action,
				  int mods)
{
	static int once = 0;
	if (once == 0)
	{
		once = 1;
		wcm_init(window_w, window_h);
	}
}

static void cursor_enter_callback(GLFWwindow* window, int entered)
{
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	mouse_x = xpos;
	mouse_y = ypos;
}

static void key_callback(GLFWwindow* window, int key, int scan, int action, int mods)
{
}

static void drop_callback(GLFWwindow* window, int num, const char** paths)
{
}

static void setup_callbacks()
{
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetCursorEnterCallback(window, cursor_enter_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetWindowPosCallback(window, window_pos_callback);

	glfwSetWindowSizeCallback(window, window_size_callback);

	//#ifndef R4_COMPAT_OPENFRAMEWORKS
	glfwSetJoystickCallback(joystick_callback);
	//#endif

	glfwSetScrollCallback(window, scroll_callback);
	glfwSetDropCallback(window, drop_callback);
}

#pragma mark callbacks

void my_tablet_prox(int v)
{
	printf("got tablet prox? %d\n", v);
}

void my_tablet_motion(double x, double y, double p, double r, double tx, double ty, double altitude, double azimuth)
{
	printf("got rich motion? %f %f %f %f %f %f\n", x, y, p, r, tx, ty);
}

void my_tablet_drag(double x, double y, double p, double r, double tx, double ty, double altitude, double azimuth)
{
	printf("got rich drag? %f %f %f %f %f %f\n", x, y, p, r, tx, ty);
}

void my_tablet_up(double x, double y, double p, double r, double tx, double ty, double altitude, double azimuth)
{
	printf("got rich up? %f %f %f %f %f %f\n", x, y, p, r, tx, ty);
}

void my_tablet_down(double x, double y, double p, double r, double tx, double ty, double altitude, double azimuth)
{
	printf("got rich down? %f %f %f %f %f %f\n", x, y, p, r, tx, ty);
}

#pragma mark main

static void draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glBegin(GL_LINES);
	glVertex2f(window_w * .5, window_h * .5);
	glVertex2f(mouse_x, mouse_y);
	glEnd();
}

int main(int argc, const char* argv[])
{

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	setup_callbacks();

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	wcm_set_tablet_proximity_func(my_tablet_prox);
	wcm_set_tablet_up_func(my_tablet_up);
	wcm_set_tablet_down_func(my_tablet_down);
	wcm_set_tablet_motion_func(my_tablet_motion);
	wcm_set_tablet_drag_func(my_tablet_drag);

	//wcm_init(window_w, window_h);

	glClearColor(1, 1, 1, 1);
	glColor4f(0,0,0,1);
	
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		draw();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
