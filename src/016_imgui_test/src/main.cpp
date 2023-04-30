
#include <fmt/format.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#define GLEW_STATIC
#include <GL/gl.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "apputils.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl2.h"
#include "camera.h"
#include "imgui.h"
#include "timing.h"

GLFWwindow* g_appWindow;
GLFWmonitor* g_appMonitor;

const GLubyte* oglRenderString;
const GLubyte* oglVersionString;
const GLubyte* oglslVersionString;

CAppState g_appState;
CPerspLookAtCamera g_Camera;
CTime g_Timer;

std::vector<glm::vec3> g_testQuad
  = { { -1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f },  { 1.0f, -1.0f, 0.0f },
      { -1.0f, 1.0f, 0.0f }, { 1.0f, -1.0f, 0.0f }, { -1.0f, -1.0f, 0.0f } };

void windowInit() {
    // g_appState = CAppState();
    g_appState.showCurrentAppState();

    if (glfwInit() != GLFW_TRUE) {
        std::cout << "windowInit(): glfwInit() return - GLFW_FALSE!"
                  << "\n";
        std::exit(1);
    }

    auto errorCallback
      = [](int, const char* err_str) { std::cout << "GLFW Error: " << err_str << std::endl; };
    glfwSetErrorCallback(errorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    g_appWindow = glfwCreateWindow(g_appState.appWindowWidth,
                                   g_appState.appWindowHeight,
                                   g_appState.appName.c_str(),
                                   nullptr,
                                   nullptr);
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

    std::cout << fmt::format(
      "windowInit():\n  oglRenderString - {}\n  oglVersionString - {}\n  oglslVersionString - {}\n",
      oglRenderString,
      oglVersionString,
      oglslVersionString);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(g_appWindow, true);
    ImGui_ImplOpenGL2_Init();
}

void registerGLFWCallbacks() {
    auto keyCallback = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    };
    glfwSetKeyCallback(g_appWindow, keyCallback);

    auto mouseButtonCallback = [](GLFWwindow* window, int button, int action, int mods) {

    };
    glfwSetMouseButtonCallback(g_appWindow, mouseButtonCallback);

    auto cursorPosCallback = [](GLFWwindow* window, double posX, double posY) {
        // g_curPositionX = posX;
        // g_curPositionY = posY;
    };
    glfwSetCursorPosCallback(g_appWindow, cursorPosCallback);

    auto cursorEnterCallback = [](GLFWwindow* window, int entered) {

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

    g_Camera.setLookPoints(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    g_Camera.setViewParameters(45.0f, g_appState.appWindowAspect, 0.01f, 100.0f);
}

void appLoop() {
    uint64_t frameBeginTime{};
    float v_frameTime{};
    uint32_t frameCount{}, v_fps{};
    CTimeDelay oneSecondDelay(1000);

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    while (!glfwWindowShouldClose(g_appWindow)) {
        glfwPollEvents();
        // -----------------------------------------------------------
        // Отрисовка тел
        // -----------------------------------------------------------

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        g_Camera.updateViewMatrix();
        glLoadMatrixf(g_Camera.getCmrMatrixPointer());

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glColor3f(0.3f, 0.6f, 1.0f);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &g_testQuad[0]);
        glDrawArrays(GL_TRIANGLES, 0, g_testQuad.size());
        glDisableClientState(GL_VERTEX_ARRAY);

        // -----------------------------------------------------------
        // Отрисовка текста
        // -----------------------------------------------------------

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a
        // named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin(
              "Hello, world!");  // Create a window called "Hello, world!" and append into it.

            ImGui::Text(
              "This is some useful text.");  // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window",
                            &show_demo_window);  // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat(
              "float", &f, 0.0f, 1.0f);  // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color",
                              (float*)&clear_color);  // Edit 3 floats representing a color

            if (ImGui::Button("Button"))  // Buttons return true when clicked (most widgets return
                                          // true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                        1000.0f / ImGui::GetIO().Framerate,
                        ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        // If you are using this code with non-legacy OpenGL header/contexts (which you should not,
        // prefer using imgui_impl_opengl3.cpp!!), you may need to backup/reset/restore other state,
        // e.g. for current shader using the commented lines below.
        // GLint last_program;
        // glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
        // glUseProgram(0);
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        // glUseProgram(last_program);

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

int main(int argc, char* argv[]) {
    appSetup();

    appLoop();

    appDefer();

    return 0;
}
