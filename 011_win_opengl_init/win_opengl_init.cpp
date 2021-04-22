
#include <iostream>
#include <Windows.h>
#include <windowsX.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glext.h>
#include <GLFW/glfw3.h>

#include "win_opengl_init.h"
#include <algebra2.h>

using namespace std;

vec3 x_ax = vec3(1.0f, 0.0f, 0.0f);
vec3 y_ax = vec3(0.0f, 1.0f, 0.0f);
vec3 z_ax = vec3(0.0f, 0.0f, 1.0f);
mtrx3 m_rtn;
qtnn q_rtn; 

void draw_decart(float scale) {
	
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(scale, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, scale, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, scale);
	glEnd();
	
}

/* ================================ */
/* 				box_c				*/
/*================================= */

void box_c::append(const vec3 &pos, const mtrx3 &spd) {
	orientation.push_back(tuple{pos, spd, mtrx3()});
}

void box_c::show() {
	uint8_t j;

	for (auto &it : orientation) {

		for (j = 0; j < 8; j++) {
			/* поворачиваем, умножая вершину на матрицу положения */
			clone[j] = mtrx3_mult_vec(get<2>(it), base[j]);
			/* переносим, склалдывая вершину с вектором смещения */ 
			clone[j] = vec3Sum(get<0>(it), clone[j]);
		}

		for (j = 0; j < 6; j++) {
			/* поворачиваем, умножая вершину на матрицу положения */
			clone_normal[j] = mtrx3_mult_vec(get<2>(it), base_normal[j]);
		}

		/* изменяем положение, умножая матрицу положения на матрицу скорости вращения */ 
		get<2>(it) = mtrx3_mult(get<2>(it),
								get<1>(it));

		glBegin(GL_QUADS);

		glColor3f(0.1f, 0.6f, 0.1f);
		glNormal3fv(&clone_normal[0][0]);
		glVertex3fv(&clone[0][0]);
		glVertex3fv(&clone[1][0]);
		glVertex3fv(&clone[2][0]);
		glVertex3fv(&clone[3][0]);

		glColor3f(0.8f, 0.9f, 0.1f);
		glNormal3fv(&clone_normal[1][0]);
		glVertex3fv(&clone[4][0]);
		glVertex3fv(&clone[5][0]);
		glVertex3fv(&clone[6][0]);
		glVertex3fv(&clone[7][0]);

		glColor3f(0.0f, 1.0f, 1.0f);
		glNormal3fv(&clone_normal[2][0]);
		glVertex3fv(&clone[0][0]);
		glVertex3fv(&clone[1][0]);
		glVertex3fv(&clone[5][0]);
		glVertex3fv(&clone[4][0]);

		glColor3f(0.93f, 0.11f, 0.23f);
		glNormal3fv(&clone_normal[3][0]);
		glVertex3fv(&clone[2][0]);
		glVertex3fv(&clone[3][0]);
		glVertex3fv(&clone[7][0]);
		glVertex3fv(&clone[6][0]);

		glColor3f(0.1f, 0.15f, 0.75f);
		glNormal3fv(&clone_normal[4][0]);
		glVertex3fv(&clone[0][0]);
		glVertex3fv(&clone[4][0]);
		glVertex3fv(&clone[7][0]);
		glVertex3fv(&clone[3][0]);

		glColor3f(0.8f, 0.2f, 0.98f);
		glNormal3fv(&clone_normal[5][0]);
		glVertex3fv(&clone[1][0]);
		glVertex3fv(&clone[5][0]);
		glVertex3fv(&clone[6][0]);
		glVertex3fv(&clone[2][0]);

		glEnd();
	}
}

/* ================================ */
/* 			app_glfw_c				*/
/*================================= */

void app_glfw_c::get_gl_properties() {
	gl_render = gl_render + "GL_RENDER - " + (const char*)glGetString(GL_RENDERER);
	gl_version = gl_version + "GL_VERSION - " + (const char*)glGetString(GL_VERSION);
	glsl_version = glsl_version + "GLSL_VERSION - " + (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
}

void app_glfw_c::print_gl_properties() {
	std::cout << gl_render << std::endl << gl_version << std::endl << glsl_version << std::endl;
}

void app_glfw_c::setup() {
	vec4 ambient = vec4{0.5, 0.5, 0.5, 1};
	vec4 diffuse = vec4{1, 1, 1, 1};
	vec4 lightPosition = vec4{0, 0, 10, 1};
	
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

	box.append(vec3(4.0, 0.0, 5.0), mtrx3(   0.1,  0.0,   0.1));
	box.append(vec3(-4.0, 0.0, 0.0), mtrx3(  0.1, -0.2,   0.2));
	box.append(vec3(0.0, 3.0, 6.0), mtrx3(  -0.1,  0.15,  0.0));
	box.append(vec3(3.0, 6.0, 0.0), mtrx3(   0.1, -0.1,   0.3));
	box.append(vec3(-4.0, -6.0, 4.0), mtrx3( 0.1,  0.2,  -0.1));
	box.append(vec3(-7.0, -5.0, 2.0), mtrx3( 0.2,  0.15, 0.1));
	box.append(vec3(7.0, 5.0, 3.0), mtrx3(  -0.2, -0.1,  -0.2));
	box.append(vec3(-8.0, 6.0, 0.0), mtrx3( -0.2,  0.1,  0.1));
}

void app_glfw_c::frame() {
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

void app_glfw_c::init_app(int argc, char* argv[]) {
	
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

	//glewExperimental = GL_TRUE;
	
	//if (glewInit() != GLEW_OK) {
	//    std::cout << "mainApp_glfw_c::init_app(): Failed to initialize GLEW" << std::endl;
	//    exit(1);
	//};

	get_gl_properties();
	print_gl_properties();

	setup();
}

void app_glfw_c::looper() {
	while(!glfwWindowShouldClose(window)) {
    	glfwPollEvents();
   		    	
		frame();

    	glfwSwapBuffers(window);
	}
}

app_glfw_c::~app_glfw_c() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void render(GLFWwindow* wnd)
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glBegin(GL_TRIANGLES);
		glVertex3f( 0.0f,  0.5f,  0.0f);
		glVertex3f( 0.5f, -0.5f,  0.0f);
		glVertex3f( -0.5f, -0.5f, 0.0f);
	glEnd();
	
	glfwSwapBuffers(wnd);
}

/* ================================ */
/* 			main()					*/
/*================================= */

int main(int argc, char *argv[])
{
	/*MSG msg;
	GLFWwindow* window;
	GLuint width = 640;
	GLuint height = 480;
	
	if (!glfwInit()) 
	{
		std::cout << "ERROR: could not start GLFW3\n";
		return msg.wParam;
	} 
	
	window = glfwCreateWindow(width, height, "Hello Triangle", NULL, NULL);
	
	if (!window) 
	{
		std::cout << "ERROR: could not open window with GLFW3\n";
		glfwTerminate();
		return msg.wParam;
	}
	
	glfwMakeContextCurrent(window);
	
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION); 
	std::cout << "Renderer: " << renderer << std::endl;
	std::cout << "OpenGL version supported " << version << std::endl;;
	
	glEnable(GL_DEPTH_TEST); 
	glDepthFunc(GL_LESS); 
	glViewport(0, 0, width, height);
	
	while(GetMessage(&msg,NULL,0,0))
	{
		switch(msg.message)
		{
			case WM_KEYDOWN:
				switch (msg.wParam)
				{
					case VK_ESCAPE:
					{
						PostQuitMessage(0);
						return 0;
						break;
					}
					case VK_SPACE:
					{
						PostQuitMessage(0);
						return 0;
						break;
					}
				}
		}
		
		render(window);
		
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	
	return msg.wParam;*/
	app_glfw_c app;

	app.init_app(argc,argv);

	app.looper();
}



