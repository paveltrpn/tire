
module;

#include <string>
#include <iostream>
#include <format>
#include <print>

#include "nlohmann/json.hpp"

export module config;

namespace tire {

export struct Config {
        Config() = default;

        // clangd: modernize-pass-by-value! WAT???
        Config(const nlohmann::json& json) : json_{ json } {
        }

        [[nodiscard]]
        std::string getString(std::string_view param) const {
            return json_[param];
        }

        [[nodiscard]]
        int getInt(std::string_view param) const {
            return json_[param];
        }

        [[nodiscard]]
        bool getBool(std::string_view param) const {
            return json_[param];
        }

    private:
        nlohmann::json json_;
};

}  // namespace tire