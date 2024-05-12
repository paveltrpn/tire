

#ifndef __renderlegacygl_h__
#define __renderlegacygl_h__

#include "Render.h"

namespace tire {

struct RenderLegacyGL final : Render {
        RenderLegacyGL();
        ~RenderLegacyGL() override;

        void displayRenderInfo() override;

        void appendToRenderList(std::shared_ptr<tire::Node<point_scalar_type>> node) override;

    private:
        void configureGl();

        void setupDebugMessages();

        void preFrame() override;
        void postFrame() override;
        void swapBuffers() override;

        void traverse() override {};

        GLXContext glContext_{ nullptr };

        std::string vendor_;
        std::string renderer_;
        std::string glVersion_;
        std::string glslVersion_;
};

}  // namespace tire
#endif
