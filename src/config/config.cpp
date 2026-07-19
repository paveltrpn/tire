
#include "config.h"

namespace tire {

auto Config::getBasePath() const -> const std::filesystem::path& {
    //
    return basePath_;
};

auto Config::getString( std::string_view param ) const -> std::string {
    //
    return config_[param];
}

auto Config::getBool( std::string_view param ) const -> bool {
    //
    return config_[param];
}

auto Config::getNumber( std::string_view param ) const -> double {
    //
    return config_[param];
}

auto Config::getJson( std::string_view param ) const -> nlohmann::json {
    //
    return config_[param];
}

}  // namespace tire