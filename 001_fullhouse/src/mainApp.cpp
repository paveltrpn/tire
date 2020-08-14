
#include "mainApp.h"
#include "algebra2.h"
#include "mesh.h"
#include "timer.h"

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
	vec4_t diffuse = vec4_t{1, 1, 1, 1};
	vec4_t lightPosition = vec4_t{0, 0, 10, 1};
	
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
	
	box.append(vec3_t(4.0, 0.0, 5.0), mtrx3_t(   0.1,  0.0,   0.1));
	box.append(vec3_t(-4.0, 0.0, 0.0), mtrx3_t(  0.1, -0.2,   0.2));
	box.append(vec3_t(0.0, 3.0, 6.0), mtrx3_t(  -0.1,  0.15,  0.0));
	box.append(vec3_t(3.0, 6.0, 0.0), mtrx3_t(   0.1, -0.1,   0.3));
	box.append(vec3_t(-4.0, -6.0, 4.0), mtrx3_t( 0.1,  0.2,  -0.1));
	box.append(vec3_t(-7.0, -5.0, 2.0), mtrx3_t( 0.2,  0.15, 0.1));
	box.append(vec3_t(7.0, 5.0, 3.0), mtrx3_t(  -0.2, -0.1,  -0.2));
	box.append(vec3_t(-8.0, 6.0, 0.0), mtrx3_t( -0.2,  0.1,  0.1));
}

void mainApp_glfw_c::frame() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, aspect, 0.1f, 100.0f); 
	glTranslatef(0.0f, 0.0f, -25.0f);
	
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
	
	box.show();
	
	glFlush();
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
   		    	
		frame();

		frames++;
		auto [watch, cdelta] = timer.watch_millisec(2000);
		if (watch) {
			std::cout << (float)frames/cdelta*1000 << " fps\n";
			frames = 0;
		}
		
    	glfwSwapBuffers(window);
	}
}

mainApp_glfw_c::~mainApp_glfw_c() {
	glfwDestroyWindow(window);
	glfwTerminate();
}
