
module;

#include <vector>
#include <initializer_list>
#include <iterator>

export module geometry : node;

import toy_std;
import : polytope;
import : point;
import : normal;

using namespace toy::algebra;

namespace tire
{
export {

    struct Node {
        Node() = default;

        Node( const Polytope &body ) {
            vertecies_ = body.getVertecies();
            indices_ = body.getIndices();
        }

        /*
             * return vertecies count, stored in vertecies array
             */
        [[nodiscard]] size_t getVerteciesCount() const { return vertecies_.size(); }

        /*
             * return vertecies array size in bytes
             */
        [[nodiscard]] size_t getVerteciesArraySize() const { return getVerteciesCount() * 3 * sizeof( double ); }

        auto getVerteciesData() { return vertecies_.data(); }

        auto getIndeciessData() { return indices_.data(); }

        void setOffset( vector3d offst ) {
            offset_ = offst;
            dirty_ = true;
        }

        void setRotate( matrix3d rtn ) {
            rotation_ = rtn;
            dirty_ = true;
        }

        void setScale( matrix3d scl ) {
            scale_ = scl;
            dirty_ = true;
        }

        void applyRotate() {
            if ( dirty_ ) {
                for ( auto i = 0; i < vertecies_.size(); ++i ) {
                    //vertecies_[i].transform( rotation_ );
                }
                dirty_ = false;
            }
        }

        void applyScale() {
            if ( dirty_ ) {
                for ( auto i = 0; i < vertecies_.size(); ++i ) {
                    //vertecies_[i].transform( scale_ );
                }
                dirty_ = false;
            }
        }

    private:
        bool dirty_{ false };
        bool useIndecies_{ false };

        std::vector<point3d> vertecies_;
        std::vector<long long> indices_;
        std::vector<vector3f> colors_;
        std::vector<vector2f> texCoords_;
        std::vector<normald> normals_;

        vector3d offset_{};
        matrix3d rotation_{};
        matrix3d scale_{};
    };
}
} // namespace tire
