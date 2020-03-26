
#ifndef __mainApp_h_
#define __mainApp_h_

#include <iostream>
#include <string>

#include <SDL2/SDL.h>

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GLFW/glfw3.h>


#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysymdef.h>

#include <algebra2.h>
#include "mesh.h"

class mainApp_c {
	protected:
		bool is_run;
		uint32_t x_pos, y_pos, x_res, y_res;
		const char* appName; /* const что бы избежать "ISO C++ forbids converting a string constant to ‘char*’" */ 
		float aspect;
		std::string gl_render, gl_version, glsl_version;
		box_c box;
		
		virtual void get_gl_properties();
		virtual void print_gl_properties();
		virtual void setup();
		virtual void frame();

	public:
		virtual void init_app(int argc, char* argv[]) = 0;
		virtual void looper() = 0;
};

class mainApp_sdl_c : mainApp_c {
	private:
		SDL_Window *window;
		SDL_GLContext glcontext;

	public:
		mainApp_sdl_c() {
			x_pos = SDL_WINDOWPOS_CENTERED;
			y_pos = SDL_WINDOWPOS_CENTERED;
			x_res = 800;
			y_res = 600;
			appName = "app";

			std::cout << "main(): Using SDL init!" << std::endl;
		}

		~mainApp_sdl_c();

		void init_app(int argc, char* argv[]);
		void looper();
};

class mainApp_X11_c : mainApp_c {
	private:
		Display* display;
		Window window;
		Screen* screen;
		int screenId;
		XEvent ev;
		Atom atomWmDeleteWindow;
		GLXContext context;
		XVisualInfo* visual;
		XSetWindowAttributes windowAttribs;
		
	public:
		mainApp_X11_c() {
			x_pos = (int)1980 / 2;
			y_pos = (int)1020 / 2;
			x_res = 800;
			y_res = 600;
			appName = "app";

			std::cout << "main(): Using X11 init!" << std::endl;
		};

		~mainApp_X11_c();

		void init_app(int argc, char* argv[]);
		void looper();
};

class mainApp_glfw_c : mainApp_c {
	protected:
		GLFWwindow* window;
		bool glewExperimental;
		
	public:
		mainApp_glfw_c() {
			x_pos = (int)1980 / 2;
			y_pos = (int)1020 / 2;
			x_res = 800;
			y_res = 600;
			appName = "app";

			std::cout << "main(): Using glfw init!" << std::endl;
		};

		~mainApp_glfw_c();

		void init_app(int argc, char* argv[]);
		void looper();
};

class mainApp_glut_c : mainApp_c {
	protected:
		static void draw();
		static void reshape(int w, int h);
		
	public:
		mainApp_glut_c() {
			x_pos = (int)1980 / 2;
			y_pos = (int)1020 / 2;
			x_res = 800;
			y_res = 600;
			appName = "app";
		};

		~mainApp_glut_c();

		void init_app(int argc, char* argv[]);
		void looper();
};

#endif
