
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
#include "bitmap.h"

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

float a,b;
float c_viscosity  = 0.005f;
float c_waveHgt	   = 0.02f;
float c_splashHgt  = 5.0f;
float c_fieldSize = 0.975f;
int32_t c_fieldPointCount = 64;
int32_t c_fieldPointCountNew = c_fieldPointCount;
GLuint g_waterTexId;

std::vector<glm::vec3> g_fieldPoints;
std::vector<glm::vec3> g_fieldNormals;

std::vector<float> g_fieldPrev, g_fieldNext;
auto g_fPrevRef = &g_fieldPrev;
auto g_fNextRef = &g_fieldNext;

constexpr int32_t idRw(int32_t i, int32_t j, int32_t n) {
	return (i*n + j);
};

constexpr int32_t idCw(int32_t i, int32_t j, int32_t n) {
	return (j*n + i);
};

void initWater() {
	g_fieldPoints = std::vector<glm::vec3>(c_fieldPointCount*c_fieldPointCount, glm::vec3(0.0f, 0.0f, 0.0f));
	g_fieldNormals = std::vector<glm::vec3>(c_fieldPointCount*c_fieldPointCount, glm::vec3(0.0f, 0.0f, 0.0f));
	g_fieldPrev = std::vector<float>(c_fieldPointCount*c_fieldPointCount, 0.0f);
	g_fieldNext = std::vector<float>(c_fieldPointCount*c_fieldPointCount, 0.0f);

	for(size_t i = 0; i < c_fieldPointCount; i++) {
		for(size_t  j = 0; j < c_fieldPointCount; j++) {
			g_fieldPoints [idRw(i, j, c_fieldPointCount)][0] = c_fieldSize - (2.0f * c_fieldSize) * i / static_cast<float>(c_fieldPointCount-1);
			g_fieldPoints [idRw(i, j, c_fieldPointCount)][1] = c_fieldSize - (2.0f * c_fieldSize) * j / static_cast<float>(c_fieldPointCount-1);
			g_fieldNormals[idRw(i, j, c_fieldPointCount)][2] = -4.0f / static_cast<float>(c_fieldPointCount-1);
		}
	}
};
									   
void waterDoStep() {
	int i,j,i1,j1;

	i1 = std::rand() % c_fieldPointCount;
	j1 = std::rand() % c_fieldPointCount;

    if((std::rand() & (c_fieldPointCount - 1)) == 0) {
		for(i = -3; i < 4; i++) {
			for(j = -3; j < 4; j++) {
				float v = c_splashHgt - i*i - j*j;
				if(v < 0.0f) v = 0.0f;
				(*g_fNextRef)[idRw(i+i1+3, j+j1+3, c_fieldPointCount)] -= v * c_waveHgt;
			}
		}
	}

	for(i = 1; i < c_fieldPointCount - 1; i++)	{
		for(j = 1; j < c_fieldPointCount - 1; j++)	{
			g_fieldNormals[idRw(i, j, c_fieldPointCount)][0] = (*g_fNextRef)[idRw(i-1, j, c_fieldPointCount)] - (*g_fNextRef)[idRw(i+1, j, c_fieldPointCount)];
			g_fieldNormals[idRw(i, j, c_fieldPointCount)][1] = (*g_fNextRef)[idRw(i, j-1, c_fieldPointCount)] - (*g_fNextRef)[idRw(i, j+1, c_fieldPointCount)];
			g_fieldPoints [idRw(i, j, c_fieldPointCount)][2] = (*g_fNextRef)[idRw(i,j, c_fieldPointCount)];
			
			float laplas = ((*g_fNextRef)[idRw(i-1, j, c_fieldPointCount)] +
				            (*g_fNextRef)[idRw(i+1, j, c_fieldPointCount)] +
						    (*g_fNextRef)[idRw(i, j+1, c_fieldPointCount)] +
						    (*g_fNextRef)[idRw(i, j-1, c_fieldPointCount)]) * 0.25f - (*g_fNextRef)[idRw(i, j, c_fieldPointCount)];

			(*g_fPrevRef)[idRw(i, j, c_fieldPointCount)] = ((2.0f - c_viscosity) * 
												  (*g_fNextRef)[idRw(i, j, c_fieldPointCount)] - 
												  (*g_fPrevRef)[idRw(i, j, c_fieldPointCount)] * (1.0f - c_viscosity) + laplas);

		}
	}

	for(i = 1; i < c_fieldPointCount - 1; i++) {
		glBegin(GL_TRIANGLE_STRIP);
		for(j = 1; j < c_fieldPointCount - 1; j++) {
			glNormal3fv(&g_fieldNormals[idRw(i, j, c_fieldPointCount)][0]);
			glVertex3fv(&g_fieldPoints [idRw(i, j, c_fieldPointCount)][0]);
			glNormal3fv(&g_fieldNormals[idRw(i+1, j, c_fieldPointCount)][0]);
			glVertex3fv(&g_fieldPoints [idRw(i+1, j, c_fieldPointCount)][0]);
		}
		glEnd();
	}

	std::swap(g_fPrevRef, g_fNextRef);
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

	g_Camera.setLookPoints(glm::vec3(-1.5f, 0.0f, 1.5f), glm::vec3(-0.2f, 0.0f, 0.0f));
	g_Camera.setViewParameters(45.0f, g_appState.appWindowAspect, 0.01f, 100.0f);
	g_Camera.setUpVec(glm::vec3(0.0f, 0.0f, 1.0f));

	std::vector<glm::vec4> ambient =			{{0.2f, 0.2f, 0.2f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}};
	std::vector<glm::vec4> diffuse =			{{1.0f, 1.0f, 1.0f, 1.0f}, {0.7f, 0.7f, 0.7f, 1.0f}};
	std::vector<glm::vec4> lightPosition =		{{1.0f, 1.0f,-2.0f, 1.0f}, {-35.0f, -5.0f, -20.0f, 1.0f}};
	glEnable(GL_LIGHTING); 
	glShadeModel(GL_SMOOTH);
	glLightfv(GL_LIGHT0, GL_AMBIENT, &ambient[0][0]);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &diffuse[0][0]);
	glLightfv(GL_LIGHT0, GL_POSITION, &lightPosition[0][0]);

	glLightfv(GL_LIGHT1, GL_AMBIENT, &ambient[1][0]);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, &diffuse[1][0]);
	glLightfv(GL_LIGHT1, GL_POSITION, &lightPosition[1][0]);
	glEnable(GL_NORMALIZE);

	CBitmap tmpBitmap;
	tmpBitmap.readFromFile("assets/wall.jpg");

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &g_waterTexId);
	glBindTexture(GL_TEXTURE_2D, g_waterTexId);
	glTexImage2D(GL_TEXTURE_2D, 0, tmpBitmap.getChanelsCount(), tmpBitmap.getWidht(), tmpBitmap.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, tmpBitmap.getDataPtr());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	
	initWater();

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("assets/RobotoMono-Medium.ttf", 16);
}

void appLoop() {
    bool set_wireframe = false;
	bool set_textured = true;

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
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		// glEnable(GL_LIGHT1);

		if (set_textured) {
			glEnable(GL_TEXTURE_2D);
			glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glEnable(GL_TEXTURE_GEN_S);
	    	glEnable(GL_TEXTURE_GEN_T);	
			glBindTexture(GL_TEXTURE_2D, g_waterTexId);
		}

		waterDoStep();

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);

		// -----------------------------------------------------------
		// Отрисовка меню
		// -----------------------------------------------------------

        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::Begin("014_Water");                          

            ImGui::Text("Water modeling demo. Frame time - %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);               
            ImGui::Checkbox("Set wireframe", &set_wireframe);
			ImGui::Checkbox("Enable texture", &set_textured);

			if (ImGui::Button("Reset field")) {   
				c_fieldPointCount = c_fieldPointCountNew;
                initWater();
			}

			ImGui::SliderFloat("Field size", &c_fieldSize, 0.5f, 4.0f);
			ImGui::SliderInt("Field points count", &c_fieldPointCountNew, 8, 1024);
			ImGui::SliderFloat("Viscosity", &c_viscosity, 0.0001f, 0.05f);
			ImGui::SliderFloat("Wave height", &c_waveHgt, 0.0001f, 0.05f);
			ImGui::SliderFloat("Splash height", &c_splashHgt, 0.0f, 15.0f);

            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

		// -----------------------------------------------------------

		glfwSwapBuffers(g_appWindow);
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
