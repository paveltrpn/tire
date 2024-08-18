module;

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

export module config;

namespace tire {

template <typename T>
concept ConfigParamType
  = std::is_same_v<bool, std::remove_cv_t<T>> || std::is_same_v<int, std::remove_cv_t<T>>
    || std::is_same_v<float, std::remove_cv_t<T>>
    || std::is_same_v<std::string, std::remove_cv_t<T>>
    || std::is_same_v<nlohmann::json, std::remove_cv_t<T>>;

export {
    struct Config final {
            Config() = delete;

            explicit Config(const char* lines) {
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
                    instance_.reset(this);
                }
            }

            explicit Config(std::filesystem::path fname) {
                if (!instance_) {
                    std::ifstream file{ fname };
                    if (file) {
                        file >> config_;
                    } else {
                        throw std::runtime_error(
                          std::format("file not found: {}\n", fname.string()));
                    }
                    instance_.reset(this);
                }
            }

            explicit Config(const nlohmann::json& config) {
                if (!instance_) {
                    instance_.reset(this);
                }
            }

            Config(const Config& rhs) = delete;
            Config(Config&& rhs) = delete;
            Config& operator=(const Config& rhs) = delete;
            Config& operator=(Config&& rhs) = delete;

            ~Config() = default;

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
            nlohmann::json getJson(std::string_view param) const {
                return config_[param];
            }

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
                    spdlog::warn("json exception handled... config param error \"{}\", what: {}",
                                 param,
                                 e.what());
                    spdlog::warn("default value used: {}", dflt);
                    return dflt;
                }
            }

            static Config* instance() {
                assert(instance_);
                return instance_.get();
            }

        private:
            static std::unique_ptr<Config> instance_;

        private:
            nlohmann::json config_;
    };
}

std::unique_ptr<Config> Config::instance_ = nullptr;

}  // namespace tire
