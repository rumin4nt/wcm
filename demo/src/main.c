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

#include <stdbool.h>

GLFWwindow* window = NULL;

#include "primitives.h"
#include <wcm/wcm.h>

#define WIDTH 256
#define HEIGHT 256

static int    window_w = WIDTH;
static int    window_h = HEIGHT;
static double mouse_x  = 0;
static double mouse_y  = 0;

static double dpi	    = 1;
static double display_radius = 1;
static bool   down	   = false;



static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	window_h = height;
	window_w = width;

	d_setup(window_w, window_h);
}

static void window_pos_callback(GLFWwindow* window, int x, int y)
{
	mouse_x = x;
	mouse_y = y;
}

static void window_size_callback(GLFWwindow* window, int width, int height)
{

	window_h = height;
	window_w = width;

	d_setup(window_w, window_h);
}

// static void draw_joysticks(void);

static void scroll_callback(GLFWwindow* window, double x, double y)
{
}

static void mouse_button_callback(GLFWwindow* window, int button, int action,
				  int mods)
{
	//
	if (button > 0)
		return;

	down		= action;
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
	mouse_x = xpos * dpi;
	mouse_y = ypos * dpi;
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
	//glfwSetJoystickCallback(joystick_callback);
	//#endif

	glfwSetScrollCallback(window, scroll_callback);
	glfwSetDropCallback(window, drop_callback);
}

static void draw(void)
{
	d_clear();

	d_line(0, 0, mouse_x, mouse_y);
	d_push();

	d_translate(mouse_x, mouse_y, 0);

	d_line(0, 0, 32, 32);
	if (down)
	{
		d_color(0, 0, 0, 1);
	}
	else
	{
		d_color(0, 0, 0, .33333);
	}
	d_circle(display_radius);
	d_pop();
}

#pragma mark callbacks

void my_tablet_prox(int v)
{
	printf("got tablet prox? %d\n", v);
}

void my_tablet_motion(double x, double y, int b, double p, double r, double tx, double ty, double tangential, double altitude, double azimuth)
{
	printf("got rich motion?  %f %f %d %f %f %f %f %f %f %f\n", x, y, b, p, r, tx, ty, tangential, altitude, azimuth);
}

void my_tablet_drag(double x, double y, int b, double p, double r, double tx, double ty, double tangential, double altitude, double azimuth)
{
	printf("got rich drag?  %f %f %d %f %f %f %f %f %f %f\n", x, y, b, p, r, tx, ty, tangential, altitude, azimuth);
	display_radius = p * 32 * dpi;
}

void my_tablet_up(double x, double y, int b, double p, double r, double tx, double ty, double tangential, double altitude, double azimuth)
{
	printf("got rich up?  %f %f %d %f %f %f %f %f %f %f\n", x, y, b, p, r, tx, ty, tangential, altitude, azimuth);
	display_radius = p * 32 * dpi;
}

void my_tablet_down(double x, double y, int b, double p, double r, double tx, double ty, double tangential, double altitude, double azimuth)
{
	printf("got rich down? %f %f %d %f %f %f %f %f %f %f\n", x, y, b, p, r, tx, ty, tangential, altitude, azimuth);
	display_radius = p * 32 * dpi;
}

#pragma mark main

int main(int argc, const char* argv[])
{

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(window_w, window_h, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	setup_callbacks();

	wcm_set_tablet_proximity_func(my_tablet_prox);
	wcm_set_tablet_up_func(my_tablet_up);
	wcm_set_tablet_down_func(my_tablet_down);
	wcm_set_tablet_motion_func(my_tablet_motion);
	wcm_set_tablet_drag_func(my_tablet_drag);

	wcm_init(window_w, window_h);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	int ww, wh;
	glfwGetWindowSize(window, &ww, &wh);
	int fw, fh;
	glfwGetFramebufferSize(window, &fw, &fh);

	dpi = (double)fw / ww;

	d_set_dpiscale(dpi);
	printf("dpi: %f\n", dpi);

	d_setup(window_w, window_h);

	d_color_clear(1, 1, 1, 1);
	d_color(0, 0, 0, 1);

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
	wcm_deinit();
}
