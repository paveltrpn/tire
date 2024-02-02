
module;

#include <GLFW/glfw3.h>

export module render:Render;

import config;

namespace tire {

export struct Render {
        Render() = default;

        Render(const tire::Config& config) : config_{ config } {
        }

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
};

}  // namespace tire
