
#ifndef __screen_h__
#define __screen_h__

#include "Config.h"
#include "Render.h"

namespace tire {

enum class RenderType {
    OPENGL,
    VULKAN,
    SOFTWARE,
};

struct Screen {
        Screen() = default;

        Screen(const Config& config);

        Screen(const Screen& rhs) = delete;
        Screen(Screen&& ths) = delete;

        Screen& operator=(const Screen& rhs) = delete;
        Screen& operator=(Screen&& rhs) = delete;

        virtual ~Screen() = default;

        void setWindowWidth(unsigned int width) noexcept;

        void setWindowHeight(unsigned int height) noexcept;

        void setWindowPosX(unsigned int posX) noexcept;

        void setWindowPosY(unsigned int posY) noexcept;

        [[nodiscard]] unsigned int getWindowWidth() noexcept;

        [[nodiscard]] unsigned int getWindowHeight() noexcept;

        [[nodiscard]] unsigned int getWindowPosX() noexcept;

        [[nodiscard]] unsigned int getWindowPosY() noexcept;

        [[nodiscard]]
        Render* getRenderPtr() const;

        virtual void displayScreenInfo() = 0;

        virtual void initRender(RenderType renderType, const tire::Config& config) = 0;

        void run();

    private:
        bool run_{ true };

    protected:
        virtual bool isRun() = 0;
        virtual void preFrame() = 0;
        virtual void postFrame() = 0;

        tire::Config config_;

        std::unique_ptr<Render> render_;

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