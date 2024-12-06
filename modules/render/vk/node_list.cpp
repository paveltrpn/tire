
#include <vulkan/vulkan.h>

#include "rendervk.h"
#include "geometry/node.h"

namespace tire {

void RenderVK::appendToRenderList( std::shared_ptr<tire::Node> node ) {
    renderList_.push_back( node );
}

}  // namespace tire