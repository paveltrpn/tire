
#include <iostream>
#include <string>
// Подключать GL именно в таком порядке!
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "algebra2.h"
#include "bitmap_text.h"
#include "bitmap.h"

GLenum check_gl_error(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}
#define check_gl_error() check_gl_error(__FILE__, __LINE__) 

void glfw_error_callback(int, const char* err_str){
	std::cout << "GLFW Error: " << err_str << std::endl;
}

class app_c {
	public:

		int32_t wnd_width = 1024;
		int32_t wnd_height = 768;
		float aspect;

		GLFWwindow* window;
		bool glewExperimental;
		std::string gl_render, gl_version, glsl_version, glfw_version;

		const char* appName = "005_rocket"; /* const что бы избежать "ISO C++ forbids converting a string constant to ‘char*’" */ 

		bitmap_text_c text;

		void start_window() {
			std::cout << "app_c::start_window()\n";

			if (glfwInit() != GLFW_TRUE) {
				std::cout << "app_c::start_window(): glfwInit() return - GLFW_FALSE!" << "\n";
				exit(1);
			}

			glfwSetErrorCallback(glfw_error_callback);

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
			glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
			glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

			window = glfwCreateWindow(wnd_width, wnd_height, appName, nullptr, nullptr);
			if (window == nullptr) {
				std::cout << "app_c::start_window(): glfwCreateWindow(): Failed to create GLFW window" << std::endl;
				glfwTerminate();
				exit(1);
			}

			aspect = (float) wnd_width / (float) wnd_height;

			glfwMakeContextCurrent(window);

			glewExperimental = GL_TRUE;
	
			if (glewInit() != GLEW_OK) {
	    		std::cout << "app_c::start_window(): glewInit(): Failed to initialize GLEW" << std::endl;
	    		exit(1);
			}

			gl_render = gl_render + "GL_RENDER - " + (const char*)glGetString(GL_RENDERER);
			gl_version = gl_version + "GL_VERSION - " + (const char*)glGetString(GL_VERSION);
			glsl_version = glsl_version + "GLSL_VERSION - " + (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
			glfw_version = glfw_version + "GLFW_VERSION - " + (const char*)glfwGetVersionString();

			std::cout << gl_render << std::endl 
					  << gl_version << std::endl 
					  << glsl_version << std::endl
					  << glfw_version << std::endl;

			// Выключаем вертикальную синхронизацию (VSYNC)
			glfwSwapInterval(0);
		};

		void setup_scene() {
			std::cout << "app_c::setup_scene()\n";

			text.load_font("assets/RobotoMono-2048-1024-64-128.tga");

			// Тест libjpeg-cmake
			bitmap_c jpeg;
			jpeg.load_jpg("assets/texture.jpg");
			jpeg.show_info();
		};

		void looper() {
			while(!glfwWindowShouldClose(window)) {
    			glfwPollEvents();
   		    	
				frame();

				glfwSwapBuffers(window);
			}
		};

		app_c() {

		};

		~app_c() {
			glfwDestroyWindow(window);
			glfwTerminate();
		};

	private:

		app_c(const app_c& app) = delete;
		app_c(const app_c&& app) = delete;
		app_c& operator=(const app_c& app) = delete;
		app_c& operator=(const app_c&& app) = delete;

		void frame() {
			// ----------------
			// Отрисовка текста
			// ----------------
			glDisable(GL_LIGHTING);
			glDisable(GL_DEPTH_TEST);  
			text.set_text_position(-10.0f, 8.0f);
			text.draw_string("Test text string!");
		};
};

int main(int argc, char* argv[]) {
	app_c app;

	app.start_window();

	app.setup_scene();

	app.looper();

	return 0;
}

