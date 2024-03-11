
#ifndef __render_h__
#define __render_h__

#include <GLFW/glfw3.h>
#include "Config.h"

namespace tire {

struct Render {
        Render() = default;

        Render(const tire::Config& config);

        Render(const Render& rhs) = delete;
        Render(Render&& ths) = delete;

        Render& operator=(const Render& rhs) = delete;
        Render& operator=(Render&& rhs) = delete;

        virtual ~Render() = default;

        virtual void displayRenderInfo() = 0;

        virtual void preFrame() = 0;
        virtual void postFrame() = 0;

    protected:
        tire::Config config_;

        bool doublebuffer_;
};

}  // namespace tire

#endif