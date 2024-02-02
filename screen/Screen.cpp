
module;

#include <iostream>
#include <format>
#include <print>
#include <string>

import config;
import render;

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
            width_ =  config.getInt("window_width");
            height_ = config.getInt("window_height");
            posX_ = config.getInt("window_pos_x");
            posY_ = config.getInt("window_pos_y");
            windowAspect_ = static_cast<float>(width_) / static_cast<float>(height_);
        };

        Screen(const Screen& rhs) = delete;
        Screen(Screen&& ths) = delete;

        Screen& operator=(const Screen& rhs) = delete;
        Screen& operator=(Screen&& rhs) = delete;

        virtual ~Screen() {
            delete render_;
        };

        virtual void initRender(RenderType renderType, const tire::Config& config) = 0;
        virtual void run() = 0;

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

        virtual void displayScreenInfo() = 0;

        virtual void displayRenderInfo() {
            render_->displayRenderInfo();
        }

    protected:
        tire::Config config_;

        Render* render_;

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
