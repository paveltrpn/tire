
#ifndef __tire_node_h__
#define __tire_node_h__

#include <vector>
#include <initializer_list>

#include "glm/vec2.hpp"
#include "glm/mat4x4.hpp"

#include "Point.h"
#include "Normal.h"

namespace tire {

template <typename pType_ = tire::point3f>
struct node {
        void setVertecies(std::vector<pType_>::const_iterator start,
                          std::vector<pType_>::const_iterator end) {
            std::copy(start, end, begin(vertecies_));
        }

        void setVertecies(std::initializer_list<pType_> values) {
            std::copy(begin(values), end(values), begin(vertecies_));
        }

        void setIndices_(std::vector<long long>::const_iterator start,
                         std::vector<long long>::const_iterator end) {
            std::copy(start, end, begin(indices_));
        }
        void setIndices_(std::initializer_list<long long> values) {
            std::copy(begin(values), end(values), begin(indices_));
        }

        auto getVerteciesData() {
            return vertecies_.data();
        }

        auto getIndeciessData() {
            return indices_.data();
        }

    private:
        bool dirty_{ false };

        std::vector<pType_> vertecies_;
        std::vector<long long> indices_;
        std::vector<glm::vec3> colors_;
        std::vector<glm::vec2> texCoords_;
        std::vector<tire::normalf> normals_;

        glm::vec3 pivot_{};
        glm::mat4x4 transform_{};
};

}  // namespace tire

#endif