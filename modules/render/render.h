
#pragma once

#include <memory>
#include <list>
#include <print>
#include <filesystem>

#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "uv.h"

#include "timer.h"
#include "scene/scene.h"

namespace tire {

namespace event {

template <typename T>
struct Task;

}

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

protected:
    virtual void preLoop() = 0;
    virtual void preFrame() = 0;
    virtual void frame() = 0;
    virtual void postFrame() = 0;
    virtual void swapBuffers() = 0;
    virtual void postLoop() = 0;

    virtual void setSwapInterval( int interval ) = 0;

    void keyPressEvent( unsigned int key );
    void keyReleaseEvent( unsigned int key );
    void mouseButtonPressEvent( unsigned int key );
    void mouseButtonReleaseEvent( unsigned int key );

    // Call when mouse moves free upon window. "x" and "y"
    // represent current cursor position in window coordinates
    void mouseMoveEvent( unsigned int x, unsigned int y );
    // Call when mouse holds in defined position. "x" and "y"
    // represent current cursor ofssets.
    void mouseOffsetEvent( unsigned int x, unsigned int y );

    event::Task<void> executeByTimeOut( long long timeout );
    event::Task<void> watchFile( std::string path );
    event::Task<void> showMetrics();

protected:
    bool run_{ true };
    Timer timer_{};

    // X11
    Display *display_;
    Window window_;
    Colormap colorMap_;
    GLXFBConfig bestFbc_;

    bool holdMouse_{ false };
    unsigned int holdMouseX_{ 500 };
    unsigned int holdMouseY_{ 500 };

    // window properties
    int posx_{};
    int posy_{};
    int width_{};
    int height_{};
    
    // The Scene
    std::shared_ptr<Scene> scene_{};

private:
    void configureX11();

private:
    static void loop( uv_timer_t *handle );
};

}  // namespace tire
