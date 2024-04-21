
#ifndef __render_h__
#define __render_h__

#include <GLFW/glfw3.h>
#include "config/Config.h"

#include <GL/gl.h>
#include <GL/glx.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace tire {

struct Render {
        Render() = delete;

        Render(const tire::Config& config);

        Render(const Render& rhs) = delete;
        Render(Render&& ths) = delete;

        Render& operator=(const Render& rhs) = delete;
        Render& operator=(Render&& rhs) = delete;

        virtual ~Render();

        virtual void displayRenderInfo() = 0;

        virtual void preFrame() = 0;
        virtual void postFrame() = 0;
        virtual void swapBuffers() = 0;

        void setWindowWidth(unsigned int width) noexcept {
            width_ = width;
        };

        void setWindowHeight(unsigned int height) noexcept {
            height_ = height;
        };

        void setWindowPosX(unsigned int posX) noexcept {
            posX_ = posX;
        };

        void setWindowPosY(unsigned int posY) noexcept {
            posY_ = posY;
        };

        [[nodiscard]] unsigned int getWindowWidth() noexcept {
            return width_;
        };

        [[nodiscard]] unsigned int getWindowHeight() noexcept {
            return height_;
        };

        [[nodiscard]] unsigned int getWindowPosX() noexcept {
            return posX_;
        };

        [[nodiscard]] unsigned int getWindowPosY() noexcept {
            return posY_;
        };

        void run();
        void frame();

    private:
        void initConfig(const tire::Config& config);
        void configureX11();

    protected:
        bool run_{ true };
        tire::Config config_;

        // X11
        Display* display_;
        Window window_;
        Colormap colorMap_;
        GLXFBConfig bestFbc_;

        bool doublebuffer_;
        std::string appName_;
        bool fullscreen_;
        bool resizeable_;
        unsigned int width_;
        unsigned int height_;
        unsigned int posX_;
        unsigned int posY_;
        float windowAspect_;
};

}  // namespace tire

#endif