
#include "mainApp.h"
#include "algebra2.h"
#include "mesh.h"
#include "timer.h"
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

/* ========================================= */
/* ============ mainApp_glfw_c ============== */
/* ========================================= */

void mainApp_glfw_c::get_gl_properties() {
	gl_render = gl_render + "GL_RENDER - " + (const char*)glGetString(GL_RENDERER);
	gl_version = gl_version + "GL_VERSION - " + (const char*)glGetString(GL_VERSION);
	glsl_version = glsl_version + "GLSL_VERSION - " + (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
}

void mainApp_glfw_c::print_gl_properties() {
	std::cout << gl_render << std::endl << gl_version << std::endl << glsl_version << std::endl;
}

void mainApp_glfw_c::setup() {
	vec4_t ambient = vec4_t{0.5, 0.5, 0.5, 1};
	vec4_t diffuse = vec4_t{2, 2, 2, 2};
	vec4_t lightPosition = vec4_t{0, 0, 10, 1};
	bitmap_c tex_image;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING); 
	glShadeModel(GL_SMOOTH);
	glLightfv(GL_LIGHT0, GL_AMBIENT, &ambient[0]);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &diffuse[0]);
	glLightfv(GL_LIGHT0, GL_POSITION, &lightPosition[0]);
	glEnable(GL_LIGHT0);

	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	
	tex_image.load_tga("assets/texture.tga");
	tex_image.show_info();
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	 glTexImage2D(GL_TEXTURE_2D, 0, 3, tex_image.get_widht(), tex_image.get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, tex_image.get_data_ptr());
	// gluBuild2DMipmaps(GL_TEXTURE_2D, 3, tex_image.get_widht(), tex_image.get_height(), GL_RGB, GL_UNSIGNED_BYTE, tex_image.get_data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	text.load_font("assets/RobotoMono-2048-1024-64-128.tga");

	box.append(vec3_t(4.0, 0.0, 5.0), mtrx3_t(   0.1,  0.0,   0.1));
	box.append(vec3_t(-4.0, 0.0, 0.0), mtrx3_t(  0.1, -0.2,   0.2));
	box.append(vec3_t(0.0, 3.0, 6.0), mtrx3_t(  -0.1,  0.15,  0.0));
	box.append(vec3_t(3.0, 6.0, 0.0), mtrx3_t(   0.1, -0.1,   0.3));
	box.append(vec3_t(-4.0, -6.0, 4.0), mtrx3_t( 0.1,  0.2,  -0.1));
	box.append(vec3_t(-7.0, -5.0, 2.0), mtrx3_t( 0.2,  0.15, 0.1));
	box.append(vec3_t(7.0, 5.0, 3.0), mtrx3_t(  -0.2, -0.1,  -0.2));
	box.append(vec3_t(-8.0, 6.0, 0.0), mtrx3_t( -0.2,  0.1,  0.1));
}

void glfw_error_callback(int, const char* err_str)
{
    std::cout << "GLFW Error: " << err_str << std::endl;
}

void mainApp_glfw_c::init_app(int argc, char* argv[]) {
	if (glfwInit() != GLFW_TRUE) {
		std::cout << "mainApp_glfw_c::init_app(): glfwInit() return - GLFW_FALSE!" << "\n";
		exit(1);
	}

	glfwSetErrorCallback(glfw_error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	// В VirtualBox с OpenGL 2.1 выбрасывает ошибку
	// "Context profiles are only defined for OpenGL version 3.2 and above"
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(x_res, y_res, appName, nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "mainApp_glfw_c::init_app(): Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(1);
	};

	aspect = (float) x_res / (float) y_res;

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	
	if (glewInit() != GLEW_OK) {
	    std::cout << "mainApp_glfw_c::init_app(): Failed to initialize GLEW" << std::endl;
	    exit(1);
	};

	// Выключаем вертикальную синхронизацию (VSYNC)
	glfwSwapInterval(0);

	get_gl_properties();
	print_gl_properties();

	setup();
}

void mainApp_glfw_c::looper() {
	while(!glfwWindowShouldClose(window)) {
    	glfwPollEvents();
   		    	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f, aspect, 0.1f, 100.0f); 
		glTranslatef(0.0f, 0.0f, -20.0f);

		glMatrixMode(GL_MODELVIEW); 
		glLoadIdentity();

		// ---------------
		// Отрисовка кубов
		// ---------------
		glDepthFunc(GL_LESS);
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_LIGHTING); 

		glEnable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glBindTexture(GL_TEXTURE_2D, texture);
		box.show();
		// ---------------

		// ----------------
		// Отрисовка текста
		// ----------------
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);  
		text.set_text_position(-10.0f, 8.0f);
		text.draw_string("Test text string!");

		text.set_text_position(-10.0f, 7.0f);
		text.draw_string("fps = " + std::to_string(fps));
		// ---------------

		frames++;
		auto [watch, cdelta] = timer.watch_millisec(2000);
		if (watch) {
			fps = (float)frames/cdelta*1000;
			frames = 0;
		}
		// 

		glFlush();
		glfwSwapBuffers(window);
	}
}

mainApp_glfw_c::~mainApp_glfw_c() {
	glfwDestroyWindow(window);
	glfwTerminate();
}
