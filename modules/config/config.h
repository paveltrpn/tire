
#pragma once

#include <exception>
#include <string>
#include <fstream>
#include <format>
#include <type_traits>
#include <filesystem>

#include "log/log.h"
#include "nlohmann/json.hpp"

namespace tire {

template <typename T>
concept ConfigParamType = std::is_same_v<bool, std::remove_cv_t<T>> ||
                          std::is_same_v<int, std::remove_cv_t<T>> ||
                          std::is_same_v<float, std::remove_cv_t<T>> ||
                          std::is_same_v<std::string, std::remove_cv_t<T>> ||
                          std::is_same_v<nlohmann::json, std::remove_cv_t<T>>;

struct Config final {
private:
    Config() {
        basePath_ = std::filesystem::canonical( "/proc/self/exe" )
                        .parent_path()
                        .parent_path()
                        .parent_path()
                        .parent_path();
    };

public:
    explicit Config( const std::filesystem::path &fname )
        : Config() {
        if ( !instance_ ) {
            const auto path = std::filesystem::path{ basePath_ / fname };
            std::ifstream file{ path };
            if ( file ) {
                try {
                    config_ = nlohmann::json::parse( file );
                } catch ( const nlohmann::json::parse_error &e ) {
                    log::error(
                        "config json parse error\n"
                        "message:\t{}\n"
                        "exception id:\t{}\n"
                        "byte position of error:\t{}\n",
                        e.what(), e.id, e.byte );
                }
            } else {
                throw std::runtime_error(
                    std::format( "file not found: {}\n", path.string() ) );
            }
            instance_.reset( this );
        } else {
            log::warning(
                "Config === attempt to reinitialize Config instance!" );
        }
    }

    Config( const Config &rhs ) = delete;
    Config( Config &&rhs ) = delete;
    Config &operator=( const Config &rhs ) = delete;
    Config &operator=( Config &&rhs ) = delete;

    ~Config() = default;

    [[nodiscard]] const std::filesystem::path &getBasePath() const {
        return basePath_;
    };

    [[nodiscard]] std::string getString( std::string_view param ) const {
        return config_[param];
    }

    [[nodiscard]] int getInt( std::string_view param ) const {
        return config_[param];
    }

    [[nodiscard]] bool getBool( std::string_view param ) const {
        return config_[param];
    }

    [[nodiscard]] float getFloat( std::string_view param ) const {
        return config_[param];
    }

    [[nodiscard]] nlohmann::json getJson( std::string_view param ) const {
        return config_[param];
    }

    template <ConfigParamType T>
    [[nodiscard]] T get( std::string_view param, T dflt = {} ) const {
        try {
            if ( config_.contains( param ) ) {
                return config_[param];
            } else {
                log::warning(
                    "no such config parameter \"{}\", default value used",
                    param );
                return dflt;
            }
        } catch ( nlohmann::json::exception &e ) {
            log::warning(
                "json exception handled... config param error \"{}\", what: "
                "{}, default value used",
                param, e.what() );
            return dflt;
        }
    }

    static Config *instance() {
        if ( !instance_ ) {
            log::error(
                "Config === global instance must be initialized explicitly!" );
        }
        return instance_.get();
    }

private:
    static std::unique_ptr<Config> instance_;

private:
    nlohmann::json config_;
    std::filesystem::path basePath_{};
};

}  // namespace tire
