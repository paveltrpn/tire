
module;

#include <string>
#include <iostream>
#include <fstream>
#include <format>
#include <print>
#include <type_traits>

#include "nlohmann/json.hpp"

export module config;

namespace tire {

export using json = nlohmann::json;

template <typename T>
concept ConfigParamType
  = std::is_same_v<bool, std::remove_cv_t<T>> || std::is_same_v<int, std::remove_cv_t<T>>
    || std::is_same_v<float, std::remove_cv_t<T>>
    || std::is_same_v<std::string, std::remove_cv_t<T>>
    || std::is_same_v<json, std::remove_cv_t<T>>;

export struct Config {
        Config() = default;

        explicit Config(const char* lines) {
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

        Config(std::string_view fname) {
            std::ifstream file{ fname };
            if (file) {
                file >> config_;
            } else {
                throw std::runtime_error(std::format("file not found: {}\n", fname));
            }
        }

        // clangd: modernize-pass-by-value! WAT???
        Config(const json& config) : config_{ config } {
        }

        [[nodiscard]]
        std::string getString(std::string_view param) const {
            return config_[param];
        }

        [[nodiscard]]
        int getInt(std::string_view param) const {
            return config_[param];
        }

        [[nodiscard]]
        bool getBool(std::string_view param) const {
            return config_[param];
        }

        [[nodiscard]]
        float getFloat(std::string_view param) const {
            return config_[param];
        }

        [[nodiscard]]
        json getJson(std::string_view param) const {
            return config_[param];
        }

        template <ConfigParamType T>
        [[nodiscard]]
        T get(std::string_view param) const {
            T rt;
            try {
                rt = config_[param];
            } catch (const json::exception& e) {
                std::print("config param error:\t{}\n", e.what());
            }
            return rt;
        }

    private:
        json config_;
};

}  // namespace tire