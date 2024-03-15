
#ifndef __glfwscreen_h__
#define __glfwscreen_h__

#include "Screen.h"
#include "config/Config.h"

namespace tire {

struct GLFWScreen final : Screen {
        GLFWScreen(const Config& config);
        ~GLFWScreen() override;

        void displayScreenInfo() override;

        void initRender(RenderType renderType, const tire::Config& config) override;
        bool isRun() override;

        void preFrame() override;

        void postFrame() override;

    private:
        void createWindow();

        void initOpenGL(GLFWwindow* window, const tire::Config& config);
        void initVulkan(const tire::Config& config);

        GLFWwindow* window_;

        int glfwVersionMajor_;
        int glfwVersionMinor_;
        int glfwVersionRev_;
        std::string glfwVersionString_;
};

}  // namespace tire

#endif