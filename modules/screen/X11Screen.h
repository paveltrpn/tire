
#ifndef __x11screen_h__
#define __x11screen_h__

#include <iostream>
#include <format>
#include <print>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "config/Config.h"
#include "render/Render.h"
#include "Screen.h"

namespace tire {

struct X11Screen final : Screen {
        X11Screen(const Config& config);
        ~X11Screen();

        void displayScreenInfo() override;
        void initRender(RenderType renderType, const Config& config) override;

        bool isRun() override;

        void preFrame() override;

        void postFrame() override;

    private:
        void initOpenGL(const tire::Config& config);

        void initVulkan(const tire::Config& config);
};

}  // namespace tire

#endif