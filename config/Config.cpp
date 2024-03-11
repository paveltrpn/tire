
#include <string>
#include <iostream>
#include <fstream>
#include <format>
#include <print>
#include <type_traits>

#include "nlohmann/json.hpp"

#include "Config.h"

namespace tire {

Config::Config(const char* lines) {
    try {
        config_ = json::parse(lines);
    } catch (const json::parse_error& e) {
        std::print("config json parse error\n"
                   "message:\t{}\n"
                   "exception id:\t{}\n"
                   "byte position of error:\t{}\n",
                   e.what(),
                   e.id,
                   e.byte);
    }
}

Config::Config(std::string_view fname) {
    std::ifstream file{ fname };
    if (file) {
        file >> config_;
    } else {
        throw std::runtime_error(std::format("file not found: {}\n", fname));
    }
}

// clangd: modernize-pass-by-value! WAT???
Config::Config(const json& config) : config_{ config } {
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
json Config::getJson(std::string_view param) const {
    return config_[param];
}

}  // namespace tire