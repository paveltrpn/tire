
#include <fstream>

#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_SCENE_CPP{ true };
#include "scene.h"

namespace tire {

Scene::Scene( const std::filesystem::path& fname ) {
    const auto path = std::filesystem::path{ fname };
    std::ifstream file{ path };
    if ( file ) {
        try {
            scene_ = nlohmann::json::parse( file );
        } catch ( const nlohmann::json::parse_error& e ) {
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
}

Scene::~Scene() {
}

void Scene::process() {
}

void Scene::appendToRenderList( std::shared_ptr<Node> node ) {
    list_.push_back( node );
}

}  // namespace tire
