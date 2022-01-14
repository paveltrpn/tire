
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <fmt/format.h>
#include "bitmap_text.h"
#include "bitmap.h"

using namespace std;

float positions[108] = {
    // top
     1.0,  1.0,  1.0, -1.0,  1.0,  1.0,  1.0,  1.0, -1.0,
    -1.0,  1.0,  1.0,  1.0,  1.0, -1.0, -1.0,  1.0, -1.0,
    // bottom
     1.0, -1.0,  1.0, -1.0, -1.0,  1.0,  1.0, -1.0, -1.0,
    -1.0, -1.0,  1.0,  1.0, -1.0, -1.0, -1.0, -1.0, -1.0,
    // front
    -1.0,  1.0,  1.0,  1.0,  1.0,  1.0, -1.0, -1.0,  1.0,
     1.0,  1.0,  1.0,  1.0, -1.0,  1.0, -1.0, -1.0,  1.0,
    // back
    -1.0,  1.0, -1.0,  1.0,  1.0, -1.0, -1.0, -1.0, -1.0,
     1.0,  1.0, -1.0,  1.0, -1.0, -1.0, -1.0, -1.0, -1.0,
    // left
    -1.0,  1.0, -1.0, -1.0,  1.0,  1.0, -1.0, -1.0,  1.0,
    -1.0, -1.0,  1.0, -1.0, -1.0, -1.0, -1.0,  1.0, -1.0,
    // right
     1.0,  1.0, -1.0,  1.0,  1.0,  1.0,  1.0, -1.0,  1.0,
     1.0, -1.0,  1.0,  1.0, -1.0, -1.0,  1.0,  1.0, -1.0
};

float normals[108] = {
    // top
    0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,
    0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,
    // bottom
    0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,
    0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,
    // front
    0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,
    0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,
    // back
    0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,
    0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,
    // left
   -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0,
   -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0,
    // right
    1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0
};

float colors[108] = {
    // top (light green)
    0.5,  0.5,  0.5, 0.5,  0.5,  0.5, 0.5,  0.5,  0.5,
    0.5,  0.5,  0.5, 0.5,  0.5,  0.5, 0.5,  0.5,  0.5,
    // bottom (dark green)
    0.5,  0.5,  0.5, 0.5,  0.5,  0.5, 0.5,  0.5,  0.5,
    0.5,  0.5,  0.5, 0.5,  0.5,  0.5, 0.5,  0.5,  0.5,
    // front (light red)
    0.5,  0.5,  0.5, 0.5,  0.5,  0.5, 0.5,  0.5,  0.5,
    0.5,  0.5,  0.5, 0.5,  0.5,  0.5, 0.5,  0.5,  0.5,
    // back  (dark red)
    0.5,  0.5,  0.5, 0.5,  0.5,  0.5, 0.5,  0.5,  0.5,
    0.5,  0.5,  0.5, 0.5,  0.5,  0.5, 0.5,  0.5,  0.5,
    // left (light blue)
    0.5,  0.5,  0.5, 0.5,  0.5,  0.5, 0.5,  0.5,  0.5,
    0.5,  0.5,  0.5, 0.5,  0.5,  0.5, 0.5,  0.5,  0.5,
    // right (dark blue)
    0.5,  0.5,  0.5, 0.5,  0.5,  0.5, 0.5,  0.5,  0.5,
    0.5,  0.5,  0.5, 0.5,  0.5,  0.5, 0.5,  0.5,  0.5,
};

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

string loadShaderSource(string fname) {
	string rt;
	ifstream inFile;
	string curString;

	inFile.open(fname);

	if (!inFile) {
		cout << "loadShaderSource(): error! can't open file - " << fname << "\n";
		return rt;
	}

    for (;getline(inFile,curString);) {
        rt = rt + curString;
    }

    inFile.close();

	return rt;
}

GLuint compileShader(GLuint type, string src) {
	GLuint shader = glCreateShader(type);
	GLchar const* files[] = {src.c_str()};
	GLint lengths[]       = {static_cast<GLint>(src.size())};

    glShaderSource(shader, 1, files, lengths);
    glCompileShader(shader);

	GLint success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if(success == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);	

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);	

		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.

		std::cout << "compileShader(): can't compile shader! error log: \n";

		for (const auto &out: errorLog) {
			cout << out;
		}

		glDeleteShader(shader); // Don't leak the shader.
		return 0;
	}

    return shader;
}

class app_c {
	public:

		int32_t wnd_width = 1024;
		int32_t wnd_height = 768;
		float aspect;

		GLFWwindow* window;
		bool glewExperimental;
		std::string gl_render, gl_version, glsl_version, glfw_version;

		std::string appName = "005_rocket";

		bitmap_text_c text;

		GLuint gl_vertBuf;
    	GLuint gl_normalBuf;
    	GLuint gl_colorBuf;

		void startWindow() {
			std::cout << fmt::format("app_c::start_window()\n");

			if (glfwInit() != GLFW_TRUE) {
				std::cout << "app_c::start_window(): glfwInit() return - GLFW_FALSE!" << "\n";
				exit(1);
			}

			glfwSetErrorCallback(glfw_error_callback);

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
			glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
			glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

			window = glfwCreateWindow(wnd_width, wnd_height, appName.c_str(), nullptr, nullptr);
			if (window == nullptr) {
				std::cout << fmt::format("app_c::start_window(): glfwCreateWindow(): Failed to create GLFW window\n");
				glfwTerminate();
				exit(1);
			}

			aspect = (float) wnd_width / (float) wnd_height;

			glfwMakeContextCurrent(window);

			glewExperimental = GL_TRUE;
	
			if (glewInit() != GLEW_OK) {
	    		std::cout << fmt::format("app_c::start_window(): glewInit(): Failed to initialize GLEW\n");
	    		exit(1);
			}

			gl_render = gl_render + "GL_RENDER - " + (const char*)glGetString(GL_RENDERER);
			gl_version = gl_version + "GL_VERSION - " + (const char*)glGetString(GL_VERSION);
			glsl_version = glsl_version + "GLSL_VERSION - " + (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
			glfw_version = glfw_version + "GLFW_VERSION - " + (const char*)glfwGetVersionString();
			
			std::cout << fmt::format("GL_RENDER - {}\nGL_VERSION - {}\nGLSL_VERSION - {}\nGLFW_VERSION - []", gl_render, gl_version, glsl_version, glfw_version);

			// Выключаем вертикальную синхронизацию (VSYNC)
			glfwSwapInterval(0);
		};

		void setupScene() {
			std::cout << fmt::format("app_c::setup_scene()\n");

			text.load_font("assets/RobotoMono-2048-1024-64-128.tga");

			// Тест libjpeg-cmake
			bitmap_c jpeg;
			jpeg.load_jpg("assets/texture.jpg");
			jpeg.show_info();

			glCreateBuffers(1, &gl_vertBuf);
        	glBindBuffer(GL_ARRAY_BUFFER, gl_vertBuf);
        	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

        	// Normal buffer
        	glCreateBuffers(1, &gl_normalBuf);
        	glBindBuffer(GL_ARRAY_BUFFER, gl_normalBuf);
        	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

        	// Now set up the colors for the vertices
        	glCreateBuffers(1, &gl_colorBuf);
        	glBindBuffer(GL_ARRAY_BUFFER, gl_colorBuf);
        	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

			auto vShader = loadShaderSource("assets/vsSource.glsl");
			auto fShader = loadShaderSource("assets/fsSource.glsl");

			// auto vs = compileShader(GL_FRAGMENT_SHADER, fShader);
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

			// Буфер вершин
            glBindBuffer(GL_ARRAY_BUFFER, gl_vertBuf);
            glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
            glEnableVertexAttribArray(0);

        	// Буфер нормалей вершин
            glBindBuffer(GL_ARRAY_BUFFER, gl_normalBuf);
            glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);
            glEnableVertexAttribArray(1); // programInfo.attribLocations.vertexNormal
	
	        // Буфер цвета вершин
            glBindBuffer(GL_ARRAY_BUFFER, gl_colorBuf);
            glVertexAttribPointer(2, 3, GL_FLOAT, false, 0, 0);
            glEnableVertexAttribArray(2);

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

	app.startWindow();

	app.setupScene();

	app.looper();

	return 0;
}

