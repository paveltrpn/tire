
#ifndef __config_h__
#define __config_h__

#include <exception>
#include <string>
#include <iostream>
#include <fstream>
#include <format>
#include <print>
#include <type_traits>
#include <filesystem>

#include "spdlog/spdlog.h"
#include "nlohmann/json.hpp"

namespace tire {

template <typename T>
concept ConfigParamType
  = std::is_same_v<bool, std::remove_cv_t<T>> || std::is_same_v<int, std::remove_cv_t<T>>
    || std::is_same_v<float, std::remove_cv_t<T>>
    || std::is_same_v<std::string, std::remove_cv_t<T>>
    || std::is_same_v<nlohmann::json, std::remove_cv_t<T>>;

struct Config final {
        Config() = default;

        explicit Config(const char* lines);
        explicit Config(std::filesystem::path fname);
        explicit Config(const nlohmann::json& config);

        Config(const Config& rhs) = delete;
        Config(Config&& rhs) = delete;
        Config& operator=(const Config& rhs) = delete;
        Config& operator=(Config&& rhs) = delete;

        ~Config() = delete;

        [[nodiscard]]
        std::string getString(std::string_view param) const;
        [[nodiscard]]
        int getInt(std::string_view param) const;

        [[nodiscard]]
        bool getBool(std::string_view param) const;

        [[nodiscard]]
        float getFloat(std::string_view param) const;

        [[nodiscard]]
        nlohmann::json getJson(std::string_view param) const;

        template <ConfigParamType T>
        [[nodiscard]]
        T get(std::string_view param, T dflt = {}) const {
            try {
                if (config_.contains(param)) {
                    return config_[param];
                } else {
                    spdlog::warn("no such config parameter \"{}\"", param);
                    spdlog::warn("default value used: {}", dflt);
                    return dflt;
                }
            } catch (nlohmann::json::exception& e) {
                spdlog::warn(
                  "json exception handled... config param error \"{}\", what: {}", param, e.what());
                spdlog::warn("default value used: {}", dflt);
                return dflt;
            }
        }

        static Config* instance() {
            assert(instance_);
            return instance_;
        }

    private:
        static Config* instance_;

    private:
        nlohmann::json config_;
};

}  // namespace tire

#endif
