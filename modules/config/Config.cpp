
#include <string>
#include <iostream>
#include <fstream>
#include <format>
#include <print>
#include <type_traits>

#include "nlohmann/json.hpp"

#include "Config.h"

namespace tire {

Config* Config::instance_ = nullptr;

Config::Config(const char* lines) {
    if (!instance_) {
        try {
            config_ = nlohmann::json::parse(lines);
        } catch (const nlohmann::json::parse_error& e) {
            std::print("config json parse error\n"
                       "message:\t{}\n"
                       "exception id:\t{}\n"
                       "byte position of error:\t{}\n",
                       e.what(),
                       e.id,
                       e.byte);
        }
        instance_ = this;
    }
}

Config::Config(std::filesystem::path fname) {
    if (!instance_) {
        std::ifstream file{ fname };
        if (file) {
            file >> config_;
        } else {
            throw std::runtime_error(std::format("file not found: {}\n", fname.string()));
        }
        instance_ = this;
    }
}

Config::Config(const nlohmann::json& config) : config_{ config } {
    if (!instance_) {
        instance_ = this;
    }
}

[[nodiscard]]
std::string Config::getString(std::string_view param) const {
    return config_[param];
}

[[nodiscard]]
int Config::getInt(std::string_view param) const {
    return config_[param];
}

[[nodiscard]]
bool Config::getBool(std::string_view param) const {
    return config_[param];
}

[[nodiscard]]
float Config::getFloat(std::string_view param) const {
    return config_[param];
}

[[nodiscard]]
nlohmann::json Config::getJson(std::string_view param) const {
    return config_[param];
}

}  // namespace tire
