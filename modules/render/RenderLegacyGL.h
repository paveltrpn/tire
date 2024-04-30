

#ifndef __renderlegacygl_h__
#define __renderlegacygl_h__

#include "Render.h"

namespace tire {

struct RenderLegacyGL : Render {
        RenderLegacyGL(const tire::Config& config);
        ~RenderLegacyGL() override;

        void displayRenderInfo() override;
        void preFrame() override;
        void postFrame() override;

        void swapBuffers() override;

    private:
        void configureGl();

        void setupDebugMessages();

        GLXContext glContext_{ nullptr };

        std::string vendor_;
        std::string renderer_;
        std::string glVersion_;
        std::string glslVersion_;
};

}  // namespace tire
#endif
