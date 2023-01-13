
#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <Windows.h>
#include <windowsX.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include <algebra2.h>

using namespace std;


class box_c {
    public:
        box_c() {};
        ~box_c() {};
		
        void append(const vec3 &pos, const mtrx3 &spd);
        void show();

    private:
        /*  кортеж хранит: 
            СМЕЩЕНИЕ (0) 
            СКОРОСТЬ ПОВОРОТА (1)
            ПОЛОЖЕНИЕ (2) */
		
        vector<tuple<vec3, mtrx3, mtrx3>> orientation;
        const vec3 base[8] = {{ 1.0, 1.0, 1.0},
	                            {-1.0, 1.0, 1.0},
	                            {-1.0,-1.0, 1.0},
	                            { 1.0,-1.0, 1.0},
	                            { 1.0, 1.0,-1.0},
	                            {-1.0, 1.0,-1.0},
	                            {-1.0,-1.0,-1.0},
	                            { 1.0,-1.0,-1.0}};
        const vec3 base_normal[6] = {{ 0.0, 0.0, 1.0},
	                                   { 0.0, 0.0,-1.0},
	                                   { 0.0, 1.0, 0.0},
	                                   { 0.0,-1.0, 0.0},
	                                   { 1.0, 0.0, 0.0},
	                                   {-1.0, 0.0, 0.0}}; 
        vec3 clone[8];
        vec3 clone_normal[6];
};

class app_glfw_c {
	protected:
		bool is_run;
		uint32_t x_pos, y_pos, x_res, y_res;
		const char* appName; /* const что бы избежать "ISO C++ forbids converting a string constant to ‘char*’" */ 
		float aspect;
		std::string gl_render, gl_version, glsl_version;
		box_c box;

        MSG msg;
	    GLFWwindow* window;
		bool glewExperimental;
		
		void get_gl_properties();
		void print_gl_properties();
		void setup();
		void frame();

	public:

        app_glfw_c() {
			x_pos = (int)1980 / 2;
			y_pos = (int)1020 / 2;
			x_res = 800;
			y_res = 600;
			appName = "app";

			std::cout << "main(): Using glfw init!" << std::endl;
		};

		~app_glfw_c();

		void init_app(int argc, char* argv[]);
		void looper();
};

void draw_decart(float scale);