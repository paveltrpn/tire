
#include <iostream>
#include <format>
#include <print>
#include <string>

#include "Config.h"
#include "Render.h"
#include "Screen.h"

namespace tire {

Screen::Screen(const Config& config) : config_{ config } {
    appName_ = config.getString("application_name");
    fullscreen_ = config.getBool("fullscreen");
    resizeable_ = config.getBool("resizeable");

    width_ = config.getInt("window_width");
    height_ = config.getInt("window_height");
    posX_ = config.getInt("window_pos_x");
    posY_ = config.getInt("window_pos_y");
    windowAspect_ = static_cast<float>(width_) / static_cast<float>(height_);
};

void Screen::setWindowWidth(unsigned int width) noexcept {
    width_ = width;
}

void Screen::setWindowHeight(unsigned int height) noexcept {
    height_ = height;
}

void Screen::setWindowPosX(unsigned int posX) noexcept {
    posX_ = posX;
}

void Screen::setWindowPosY(unsigned int posY) noexcept {
    posY_ = posY;
}

unsigned int Screen::getWindowWidth() noexcept {
    return width_;
}

unsigned int Screen::getWindowHeight() noexcept {
    return height_;
}

unsigned int Screen::getWindowPosX() noexcept {
    return posX_;
}

unsigned int Screen::getWindowPosY() noexcept {
    return posY_;
}

Render* Screen::getRenderPtr() const {
    return render_.get();
}

void Screen::run() {
    while (run_) {
        preFrame();
        render_->preFrame();

        render_->postFrame();
        postFrame();

        run_ = isRun();
    }
};

}  // namespace tire
