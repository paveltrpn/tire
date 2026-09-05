
module;

#include <atomic>
#include <mutex>
#include <string>
#include <format>
#include <type_traits>
#include <filesystem>
#include <fstream>

#include "nlohmann/json.hpp"
#include "log/log.h"

export module config : config;

namespace tire {

template <typename T>
concept ConfigParamType =
    std::is_same_v<bool, std::remove_cv_t<T>> || std::is_same_v<int, std::remove_cv_t<T>> ||
    std::is_same_v<float, std::remove_cv_t<T>> || std::is_same_v<std::string, std::remove_cv_t<T>> ||
    std::is_same_v<nlohmann::json, std::remove_cv_t<T>>;

export struct Config final {
public:
    Config( const Config& rhs ) = delete;
    Config( Config&& rhs ) = delete;
    auto operator=( const Config& rhs ) -> Config& = delete;
    auto operator=( Config&& rhs ) -> Config& = delete;

    static auto init( const std::filesystem::path& fname ) -> void {
        if ( _initSuccess ) {
            log::error()( "Warning: Singleton already initialized. Ignoring new arguments." );
        }

        std::call_once( _initFlag, [&]() {
            _instance.store( new Config( fname ) );
            _initSuccess = true;
        } );
    }
    [[nodiscard]] static auto instance() -> Config& {
        Config* ptr = _instance.load();

        if ( !ptr ) {
            throw std::logic_error( "Singleton must be initialized via init() before calling getInstance()." );
        }
        return *ptr;
    }

    [[nodiscard]] auto basePath() const -> const std::filesystem::path& {
        //
        return _basePath;
    };

    [[nodiscard]] auto getString( std::string_view param ) const -> std::string {
        //
        return _config[param];
    }

    [[nodiscard]] auto getBool( std::string_view param ) const -> bool {
        //
        return _config[param];
    }

    [[nodiscard]] auto getNumber( std::string_view param ) const -> double {
        //
        return _config[param];
    }

    [[nodiscard]] auto getJson( std::string_view param ) const -> nlohmann::json {
        //
        return _config[param];
    }

    template <ConfigParamType T>
    [[nodiscard]] auto get( std::string_view param, T dflt = {} ) const -> T {
        try {
            if ( _config.contains( param ) ) {
                return _config[param];
            } else {
                log::warning()( "no such config parameter \"{}\", default value used", param );
                return dflt;
            }
        } catch ( nlohmann::json::exception& e ) {
            log::warning()(
                "json exception handled... config param error \"{}\", what: "
                "{}, default value used",
                param, e.what() );
            return dflt;
        }
    }

private:
    explicit Config( const std::filesystem::path& fname )
        : _basePath{ std::filesystem::canonical( "/proc/self/exe" ).parent_path().parent_path() } {
        const auto path = std::filesystem::path{ _basePath / fname };

        std::ifstream file{ path };
        if ( file ) {
            try {
                _config = nlohmann::json::parse( file );
            } catch ( const nlohmann::json::parse_error& e ) {
                log::error()(
                    "config json parse error\n"
                    "message:\t{}\n"
                    "exception id:\t{}\n"
                    "byte position of error:\t{}",
                    e.what(), e.id, e.byte );
            }
        } else {
            throw std::runtime_error( std::format( "file not found: {}\n", path.string() ) );
        }
    }

    ~Config() = default;

    inline static std::atomic<Config*> _instance{ nullptr };
    inline static std::once_flag _initFlag;
    inline static bool _initSuccess{ false };

private:
    nlohmann::json _config;
    std::filesystem::path _basePath{};
};

}  // namespace tire
