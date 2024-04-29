
#ifndef __tire_node_h__
#define __tire_node_h__

#include <vector>
#include <initializer_list>
#include <iterator>

#include "Vector.h"
#include "Matrix.h"

#include "Point.h"
#include "Normal.h"

namespace tire {

template <typename scalarT_ = float>
struct node {
        using point_type = tire::point3<scalarT_>;
        using normal_type = tire::normal<scalarT_>;
        using vec2_type = tire::vec<2, scalarT_>;
        using vec3_type = tire::vec<3, scalarT_>;
        using mat3_type = tire::mat<3, scalarT_>;
        using mat4_type = tire::mat<4, scalarT_>;

        node()
            : offset_{ scalarT_{}, scalarT_{}, scalarT_{} },
              rotation_{ vec3_type{ scalarT_{ 1.0 }, scalarT_{}, scalarT_{} },
                         vec3_type{ scalarT_{}, scalarT_{ 1.0 }, scalarT_{} },
                         vec3_type{ scalarT_{}, scalarT_{}, scalarT_{ 1.0 } } },
              scale_{ vec3_type{ scalarT_{ 1.0 }, scalarT_{}, scalarT_{} },
                      vec3_type{ scalarT_{}, scalarT_{ 1.0 }, scalarT_{} },
                      vec3_type{ scalarT_{}, scalarT_{}, scalarT_{ 1.0 } } } {
        }

        void setVerteciesArray(std::vector<point_type>::const_iterator start,
                               std::vector<point_type>::const_iterator end) {
            std::copy(start, end, std::back_inserter(vertecies_));
        }

        void setVerteciesArray(std::initializer_list<point_type> values) {
            std::copy(values.begin(), values.end(), std::back_inserter(vertecies_));
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

        std::vector<point_type> vertecies_;
        std::vector<long long> indices_;
        std::vector<vec3_type> colors_;
        std::vector<vec2_type> texCoords_;
        std::vector<normal_type> normals_;

        vec3_type offset_;
        mat3_type rotation_;
        mat3_type scale_;
};

}  // namespace tire

#endif
