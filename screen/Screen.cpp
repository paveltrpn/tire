
module;

#include <iostream>
#include <format>
#include <print>
#include <string>

#include "Config.h"

#include "Render.h"

export module screen:Screen;

namespace tire {

export enum class RenderType {
    OPENGL,
    VULKAN,
    SOFTWARE,
};

export struct Screen {
        Screen() = default;

        Screen(const tire::Config& config) : config_{ config } {
            appName_ = config.getString("application_name");
            fullscreen_ = config.getBool("fullscreen");
            resizeable_ = config.getBool("resizeable");
            
            width_ = config.getInt("window_width");
            height_ = config.getInt("window_height");
            posX_ = config.getInt("window_pos_x");
            posY_ = config.getInt("window_pos_y");
            windowAspect_ = static_cast<float>(width_) / static_cast<float>(height_);
        };

        Screen(const Screen& rhs) = delete;
        Screen(Screen&& ths) = delete;

        Screen& operator=(const Screen& rhs) = delete;
        Screen& operator=(Screen&& rhs) = delete;

        virtual ~Screen() = default;

        void setWindowWidth(unsigned int width) noexcept {
            width_ = width;
        }

        void setWindowHeight(unsigned int height) noexcept {
            height_ = height;
        }

        void setWindowPosX(unsigned int posX) noexcept {
            posX_ = posX;
        }

        void setWindowPosY(unsigned int posY) noexcept {
            posY_ = posY;
        }

        [[nodiscard]] unsigned int getWindowWidth() noexcept {
            return width_;
        }

        [[nodiscard]] unsigned int getWindowHeight() noexcept {
            return height_;
        }

        [[nodiscard]] unsigned int getWindowPosX() noexcept {
            return posX_;
        }

        [[nodiscard]] unsigned int getWindowPosY() noexcept {
            return posY_;
        }

        [[nodiscard]]
        Render* getRenderPtr() const {
            return render_.get();
        }

        virtual void displayScreenInfo() = 0;

        virtual void initRender(RenderType renderType, const tire::Config& config) = 0;

        void run() {
            while (run_) {
                preFrame();
                render_->preFrame();

                render_->postFrame();
                postFrame();

                run_ = isRun();
            }
        };

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
