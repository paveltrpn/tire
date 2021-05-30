
#ifndef __mainApp_h_
#define __mainApp_h_

#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "timer.h"
#include "mesh.h"
#include "bitmap_text.h"

class mainApp_glfw_c {
	private:
		GLFWwindow* window;
		bool glewExperimental;
		
		// bool is_run;
		uint32_t x_pos, y_pos, x_res, y_res;
		const char* appName; /* const что бы избежать "ISO C++ forbids converting a string constant to ‘char*’" */ 
		float aspect;
		std::string gl_render, gl_version, glsl_version;

		GLuint texture;
		box_c box;
		
		std::vector<Body> BodyList;

		bitmap_text_c text;

		uint32_t frames = 0;
		timer_c timer;
		float fps = 0.0;

		void get_gl_properties();
		void print_gl_properties();
		void setup();

		mainApp_glfw_c(mainApp_glfw_c &other) = delete;
		mainApp_glfw_c& operator=(mainApp_glfw_c &other) = delete;

	public:
		mainApp_glfw_c() {
			x_pos = (int)1980 / 2;
			y_pos = (int)1020 / 2;
			x_res = 800;
			y_res = 600;
			appName = "app";

			fps = 0.0f;
			
			std::cout << "main(): Using glfw init!" << std::endl;
		};

		~mainApp_glfw_c();

		void init_app(int argc, char* argv[]);
		void looper();
};

#endif
