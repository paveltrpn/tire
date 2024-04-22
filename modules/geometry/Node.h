
#ifndef __tire_node_h__
#define __tire_node_h__

#include <vector>

#include "glm/vec2.hpp"

#include "Point.h"
#include "Normal.h"

namespace tire {

struct node {
    private:
        std::vector<tire::point3f> vertecies_;
        std::vector<long long> indexes_;
        std::vector<glm::vec3> colors_;
        std::vector<glm::vec2> texCoords_;
        std::vector<tire::normalf> normals_;
};

}  // namespace tire

#endif