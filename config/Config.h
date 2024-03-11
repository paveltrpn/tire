
#ifndef __config_h__
#define __config_h__

#include <string>
#include <iostream>
#include <fstream>
#include <format>
#include <print>
#include <type_traits>

#include "nlohmann/json.hpp"

namespace tire {

using json = nlohmann::json;

template <typename T>
concept ConfigParamType
  = std::is_same_v<bool, std::remove_cv_t<T>> || std::is_same_v<int, std::remove_cv_t<T>>
    || std::is_same_v<float, std::remove_cv_t<T>>
    || std::is_same_v<std::string, std::remove_cv_t<T>>
    || std::is_same_v<json, std::remove_cv_t<T>>;

struct Config {
        Config() = default;

        explicit Config(const char* lines);
        Config(std::string_view fname);

        // clangd: modernize-pass-by-value! WAT???
        Config(const json& config);

        [[nodiscard]]
        std::string getString(std::string_view param) const;
        [[nodiscard]]
        int getInt(std::string_view param) const;

        [[nodiscard]]
        bool getBool(std::string_view param) const;

        [[nodiscard]]
        float getFloat(std::string_view param) const;

        [[nodiscard]]
        json getJson(std::string_view param) const;

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

#endif