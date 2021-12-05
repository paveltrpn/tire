 
#include <iostream>
#include <fmt/format.h>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "service.h"
#include "shader.h"

void setup(const SAppState &appState) {
    glViewport(0, 0, appState.wndWidth, appState.wndHeight);

	glClearColor(0.0f, 0.1f, 0.0f, 0.0f); 
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
}

int main(int argc, char** argv) {
    SAppState appSate;
    
    initGlfwWindow(appSate);

    // std::cout << g_appSate.glfwVersionStr << std::endl;
    // std::cout << g_appSate.glVersionStr << std::endl;
    // std::cout << g_appSate.glRenderStr << std::endl;
    // std::cout << g_appSate.glslVersionStr << std::endl;

    registerGlfwCallbacks(appSate);

    setup(appSate);

    COglProgram foo;

    foo.appendShader({{GL_VERTEX_SHADER, "assets/vs.glsl"}, {GL_FRAGMENT_SHADER, "assets/fs.glsl"}});
    foo.linkProgram();

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
    ImGui_ImplGlfw_InitForOpenGL(appSate.glfwWndPtr, true);
    ImGui_ImplOpenGL3_Init();

    io.Fonts->AddFontFromFileTTF("assets/RobotoMono-Medium.ttf", 16);

    while(!glfwWindowShouldClose(appSate.glfwWndPtr)) {
		glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        if (appSate.KEY_ESCAPE) {
            glfwSetWindowShouldClose(appSate.glfwWndPtr, GLFW_TRUE);
        }

        // -----------------------------------------------------------
	    // Отрисовка меню
	    // -----------------------------------------------------------

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::Begin("019_moderngl");                          

            ImGui::Text("Frame time - %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);               
	    
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	    // -----------------------------------------------------------

		glfwSwapBuffers(appSate.glfwWndPtr);
	}

    return 0;
}