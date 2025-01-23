
#pragma once

#include <memory>
#include <list>
#include <print>
#include <filesystem>

#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "uv.h"

#include "config/config.h"
#include "scene/scene.h"

namespace tire {

struct Render {
    Render();

    Render( const Render &rhs ) = delete;
    Render( Render &&ths ) = delete;

    Render &operator=( const Render &rhs ) = delete;
    Render &operator=( Render &&rhs ) = delete;

    virtual ~Render();

    virtual void displayRenderInfo() = 0;
    virtual void scene( const std::filesystem::path &path ) = 0;
    void run();

private:
    static void loop( uv_timer_t *handle );

protected:
    virtual void preLoop() = 0;
    virtual void preFrame() = 0;
    virtual void frame() = 0;
    virtual void postFrame() = 0;
    virtual void swapBuffers() = 0;
    virtual void postLoop() = 0;

    [[nodiscard]] bool isExtensionSupported( const char *extList,
                                             const char *extension );
    virtual void setSwapInterval( int interval ) = 0;

protected:
    bool run_{ true };

    // X11
    Display *display_;
    Window window_;
    Colormap colorMap_;
    GLXFBConfig bestFbc_;

    // window properties
    int posx_{};
    int posy_{};
    int width_{};
    int height_{};

    std::shared_ptr<Scene> scene_{};

private:
    void openDisplay();
    void configureX11();
};

}  // namespace tire
