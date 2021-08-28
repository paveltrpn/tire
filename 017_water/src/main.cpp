
#include <iostream>
#include <string>
#include <fmt/format.h>
#include <thread>
#include <vector>

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "apputils.h"
#include "camera.h"
#include "timing.h"

#include <glm/glm.hpp>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl2.h"

GLFWwindow 		*g_appWindow;
GLFWmonitor		*g_appMonitor;

const GLubyte *oglRenderString;
const GLubyte *oglVersionString;
const GLubyte *oglslVersionString;

CAppState			g_appState;
CPerspLookAtCamera 	g_Camera;
CTime				g_Timer;

std::vector<glm::vec3> g_testQuad = {{-1.0f, 1.0f, 0.0f}, {1.0f,  1.0f, 0.0f}, { 1.0f, -1.0f, 0.0f},
								  	 {-1.0f, 1.0f, 0.0f}, {1.0f, -1.0f, 0.0f}, {-1.0f, -1.0f, 0.0f}};

float a,b;
float c_viscosity  = 0.005f;
float c_waveHgt	   = 0.02f;

struct vertex
{
	float coo[3];
	float nor[3];

};

struct field
{
	float U[128][128];
};

vertex vertices[128][128];
field A,B;
field *p=&A,*n=&B;

void initWater() {
	int i,j;

	memset(vertices,0,sizeof(vertices));
	memset(&A,0,sizeof(A));
	memset(&B,0,sizeof(B));

	for(i=0;i<128;i++)
	{
		for(j=0;j<128;j++)
		{
			vertices[i][j].coo[0]=1.0f-2.0f*i/127.0f;
			vertices[i][j].coo[1]=1.0f-2.0f*j/127.0f;
			vertices[i][j].nor[2]=-4.0f/127.0f;
		}
	}
};

void waterDoStep() {
	int i,j,i1,j1;

	i1=rand()%128;
	j1=rand()%128;

    if((rand()&127)==0)
	for(i=-3;i<4;i++) {
		for(j=-3;j<4;j++) {
			float v=6.0f-i*i-j*j;
			if(v<0.0f)v=0.0f;
			n->U[i+i1+3][j+j1+3]-=v*c_waveHgt;
		}
	}

	for(i=1;i<127;i++)	{
		for(j=1;j<127;j++)	{
			vertices[i][j].coo[2]=n->U[i][j];
			vertices[i][j].nor[0]=n->U[i-1][j]-n->U[i+1][j];
			vertices[i][j].nor[1]=n->U[i][j-1]-n->U[i][j+1];

			float laplas=(n->U[i-1][j]+
				          n->U[i+1][j]+
						  n->U[i][j+1]+
						  n->U[i][j-1])*0.25f-n->U[i][j];

			p->U[i][j]=((2.0f-c_viscosity)*n->U[i][j]-p->U[i][j]*(1.0f-c_viscosity)+laplas);

		}
	}

	for(i=1;i<127;i++) {
		glBegin(GL_TRIANGLE_STRIP);
		for(j=1;j<127;j++) {

			glNormal3fv(vertices[i][j].nor);
			glVertex3fv(vertices[i][j].coo);
			glNormal3fv(vertices[i+1][j].nor);
			glVertex3fv(vertices[i+1][j].coo);

		}
		glEnd();
	}

	field *sw=p;p=n;n=sw;
}

void windowInit() {
	// g_appState = CAppState();
	g_appState.showCurrentAppState();

	if (glfwInit() != GLFW_TRUE) {
		std::cout << "windowInit(): glfwInit() return - GLFW_FALSE!" << "\n";
		std::exit(1);
	}

	auto errorCallback = [] (int, const char* err_str) {
		std::cout << "GLFW Error: " << err_str << std::endl;
	};
	glfwSetErrorCallback(errorCallback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	g_appWindow = glfwCreateWindow(g_appState.appWindowWidth, g_appState.appWindowHeight, g_appState.appName.c_str(), nullptr, nullptr);
	if (g_appWindow == nullptr) {
		std::cout << "windowInit(): Failed to create GLFW window" << std::endl;
		glfwTerminate();
		std::exit(1);
	};

	glfwMakeContextCurrent(g_appWindow);

	glewExperimental = GL_TRUE;
	
	if (glewInit() != GLEW_OK) {
	    std::cout << "windowInit(): Failed to initialize GLEW" << std::endl;
	    std::exit(1);
	};

	// ВКЛ-ВЫКЛ вертикальную синхронизацию (VSYNC)
	// Лок на 60 фпс
	glfwSwapInterval(true);
	
	oglRenderString = glGetString(GL_RENDERER);
	oglVersionString = glGetString(GL_VERSION);
	oglslVersionString = glGetString(GL_SHADING_LANGUAGE_VERSION);
	
	std::cout << fmt::format("windowInit():\n  oglRenderString - {}\n  oglVersionString - {}\n  oglslVersionString - {}\n", oglRenderString, oglVersionString, oglslVersionString);

	// Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(g_appWindow, true);
    ImGui_ImplOpenGL2_Init();
}

void registerGLFWCallbacks() {
	auto keyCallback = [] (GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
	};
	glfwSetKeyCallback(g_appWindow, keyCallback);

	auto mouseButtonCallback = [] (GLFWwindow* window, int button, int action, int mods) {
		
	};
	glfwSetMouseButtonCallback(g_appWindow, mouseButtonCallback);

	auto cursorPosCallback = [] (GLFWwindow* window, double posX, double posY) {
		// g_curPositionX = posX;
		// g_curPositionY = posY;
	};
	glfwSetCursorPosCallback(g_appWindow, cursorPosCallback);

	auto cursorEnterCallback = [] (GLFWwindow* window, int entered) {
		
	};
	glfwSetCursorEnterCallback(g_appWindow, cursorEnterCallback);
}

void appSetup() {
	windowInit();
	registerGLFWCallbacks();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	g_Timer = CTime();

	g_Camera.setLookPoints(glm::vec3(-1.5f, 0.0f, 1.5f), glm::vec3(-0.2f, 0.0f, 0.0f));
	g_Camera.setViewParameters(45.0f, g_appState.appWindowAspect, 0.01f, 100.0f);
	g_Camera.setUpVec(glm::vec3(0.0f, 0.0f, 1.0f));

	std::vector<glm::vec4> ambient =			{{0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}};
	std::vector<glm::vec4> diffuse =			{{1.5f, 1.5f, 1.5f, 1.0f}, {0.7f, 0.7f, 0.7f, 1.0f}};
	std::vector<glm::vec4> lightPosition =	{{2.0f, 2.0f, 3.0f, 1.0f}, {-35.0f, -5.0f, -20.0f, 1.0f}};
	// glEnable(GL_LIGHTING); 
	// glShadeModel(GL_SMOOTH);
	// glLightfv(GL_LIGHT0, GL_AMBIENT, &ambient[0][0]);
	// glLightfv(GL_LIGHT0, GL_DIFFUSE, &diffuse[0][0]);
	// glLightfv(GL_LIGHT0, GL_POSITION, &lightPosition[0][0]);
// 
	// glLightfv(GL_LIGHT1, GL_AMBIENT, &ambient[1][0]);
	// glLightfv(GL_LIGHT1, GL_DIFFUSE, &diffuse[1][0]);
	// glLightfv(GL_LIGHT1, GL_POSITION, &lightPosition[1][0]);

	initWater();

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("assets/RobotoMono-Medium.ttf", 18);
}

void appLoop() {
	uint64_t frameBeginTime{};
	float v_frameTime{};
	uint32_t frameCount{}, v_fps{};
	CTimeDelay oneSecondDelay(1000);

    bool set_wireframe = true;

	while(!glfwWindowShouldClose(g_appWindow)) {
		glfwPollEvents();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glMatrixMode(GL_PROJECTION);
		g_Camera.updateViewMatrix();
		glLoadMatrixf(g_Camera.getCmrMatrixPointer());

		glMatrixMode(GL_MODELVIEW); 
		glLoadIdentity();

		if (set_wireframe) {
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		} else {
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		}

		glColor3f(0.3f, 0.6f, 1.0f);
		// glEnable(GL_LIGHTING);
		// glEnable(GL_LIGHT0);
		// glEnable(GL_LIGHT1);
		waterDoStep();

		// -----------------------------------------------------------
		// Отрисовка текста
		// -----------------------------------------------------------

		// Start the Dear ImGui frame
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::Begin("014_Water");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("Water modeling demo");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Set wireframe", &set_wireframe);

			ImGui::SliderFloat("Viscosity", &c_viscosity, 0.0001f, 0.05f);
			ImGui::SliderFloat("Wave height", &c_waveHgt, 0.0001f, 0.05f);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
		// Rendering
        ImGui::Render();
        // If you are using this code with non-legacy OpenGL header/contexts (which you should not, prefer using imgui_impl_opengl3.cpp!!),
        // you may need to backup/reset/restore other state, e.g. for current shader using the commented lines below.
        //GLint last_program;
        //glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
        //glUseProgram(0);
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        //glUseProgram(last_program);

		// -----------------------------------------------------------

		glfwSwapBuffers(g_appWindow);

		frameBeginTime = g_Timer.getMs();
		frameCount++;

		if (oneSecondDelay.isPassed()) {
			v_fps = frameCount;
			frameCount = 0;
			oneSecondDelay.reset();
		}

		v_frameTime = static_cast<float>(g_Timer.getMs() - frameBeginTime) / 1000.0f;
		frameBeginTime = 0;
	}
}

void appDefer() {
	// Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
	
	glfwDestroyWindow(g_appWindow);
	glfwTerminate();
}

int main(int argc, char *argv[]) {

    appSetup();
    
    appLoop();

    appDefer();

    return 0;
}
