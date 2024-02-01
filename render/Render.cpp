
module;

#include <GLFW/glfw3.h>

export module render:Render;

namespace tire {

export struct Render {
        Render() = default;

        Render(const Render& rhs) = delete;
        Render(Render&& ths) = delete;

        Render& operator=(const Render& rhs) = delete;
        Render& operator=(Render&& rhs) = delete;

        virtual ~Render() = default;

        virtual void displayRenderInfo() = 0;

        virtual void swapBuffers() = 0;

    protected:

        bool resizeable_;
};

}  // namespace tire
