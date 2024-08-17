
#ifndef __tire_node_h__
#define __tire_node_h__

#include <vector>
#include <initializer_list>
#include <iterator>

#include "Point.h"
#include "Normal.h"

import toy_std;

namespace tire {

template <typename T = float>
struct Node {
        using scalar_type = T;
        using point_type = tire::point3<scalar_type>;
        using normal_type = tire::normal<scalar_type>;
        using vec2_type = toy::algebra::vector2<scalar_type>;
        using vec3_type = toy::algebra::vector2<scalar_type>;
        using mat3_type = toy::algebra::matrix3<scalar_type>;
        using mat4_type = toy::algebra::matrix4<scalar_type>;

        Node() = default;

        void setVerteciesArray(std::vector<point_type>::const_iterator start,
                               std::vector<point_type>::const_iterator end) {
            std::copy(start, end, std::back_inserter(vertecies_));
        }

        void setVerteciesArray(std::initializer_list<point_type> values) {
            std::copy(values.begin(), values.end(), std::back_inserter(vertecies_));
        }

        /*
         * return vertecies count, stored in vertecies array
         */
        [[nodiscard]]
        size_t getVerteciesCount() const {
            return vertecies_.size();
        }

        /*
         * return vertecies array size in bytes
         */
        [[nodiscard]]
        size_t getVerteciesArraySize() const {
            return getVerteciesCount() * 3 * sizeof(scalar_type);
        }

        void setIndicesArray(std::vector<long long>::const_iterator start,
                             std::vector<long long>::const_iterator end) {
            std::copy(start, end, std::back_inserter(indices_));
        }

        void setIndicesArray(std::initializer_list<long long> values) {
            std::copy(values.begin(), values.end(), std::back_inserter(indices_));
        }

        void setNormalsArray(std::vector<normal_type>::const_iterator start,
                             std::vector<normal_type>::const_iterator end) {
            std::copy(start, end, std::back_inserter(normals_));
        }

        void setNormalsArray(std::initializer_list<normal_type> values) {
            std::copy(values.begin(), values.end(), std::back_inserter(normals_));
        }

        void setColorsArray(std::vector<vec3_type>::const_iterator start,
                            std::vector<vec3_type> end) {
            std::copy(start, end, std::back_inserter(colors_));
        }

        void setColorsArray(std::initializer_list<vec3_type> values) {
            std::copy(values.begin(), values.end(), std::back_inserter(colors_));
        }

        void setTexCoordsArray(std::vector<vec2_type>::const_iterator start,
                               std::vector<vec2_type>::const_iterator end) {
            std::copy(start, end, std::back_inserter(texCoords_));
        }

        void setTexCoordsArray(std::initializer_list<vec2_type> values) {
            std::copy(values.begin(), values.end(), std::back_inserter(texCoords_));
        }

        auto getVerteciesData() {
            return vertecies_.data();
        }

        auto getIndeciessData() {
            return indices_.data();
        }

        void setOffset(vec3_type offst) {
            offset_ = offst;
            dirty_ = true;
        }

        void setRotate(mat3_type rtn) {
            rotation_ = rtn;
            dirty_ = true;
        }

        void setScale(mat3_type scl) {
            scale_ = scl;
            dirty_ = true;
        }

        void applyRotate() {
            if (dirty_) {
                for (auto i = 0; i < vertecies_; ++i) {
                    vertecies_[i].transform(rotation_);
                }
                dirty_ = false;
            }
        }

        void applyScale() {
            if (dirty_) {
                for (auto i = 0; i < vertecies_; ++i) {
                    vertecies_[i].transform(scale_);
                }
                dirty_ = false;
            }
        }

    private:
        bool dirty_{ false };
        bool useIndecies_{ false };

        std::vector<point_type> vertecies_;
        std::vector<long long> indices_;
        std::vector<vec3_type> colors_;
        std::vector<vec2_type> texCoords_;
        std::vector<normal_type> normals_;

        vec3_type offset_{};
        mat3_type rotation_{};
        mat3_type scale_{};
};

}  // namespace tire

#endif
