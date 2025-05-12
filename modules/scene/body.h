
#pragma once

#include <vector>
#include <initializer_list>
#include <iterator>
#include <memory>

#include "geometry/polytope.h"
#include "geometry/bounding_volumes.h"

#include "algebra/vector2.h"
#include "algebra/vector3.h"
#include "algebra/matrix3.h"

#include "image/color.h"
#include "image/tga.h"

namespace tire {

using namespace algebra;

struct Body final {
    using value_type = float;

    Body() = default;

    void setShapeData( std::shared_ptr<PolytopeData> data );

    [[nodiscard]]
    size_t verteciesCount() const;
    [[nodiscard]]
    size_t verteciesArraySize() const;
    [[nodiscard]]
    size_t normalsArraySize() const;
    [[nodiscard]]
    size_t texcrdsArraySize() const;

    //
    const vector3<value_type> *verteciesData();
    const vector3<value_type> *normalsData();
    const vector2<value_type> *texcrdsData();

    void setBounding( BoundingVolume<float> value ) { bounding_ = value; }
    const BoundingVolume<float> bounding() { return bounding_; };

    void setAlbedoColor( const std::string &name ) {
        albedoColor_ = Colorf{ name };
    }

    [[nodiscard]]
    Colorf albedoColor() const {
        return albedoColor_;
    };

    void setPosition( const vector3<value_type> &value );
    void setOrientation( const vector3<value_type> &value );
    void setScale( const vector3<value_type> &value );
    void setVelocity( const vector3<value_type> &value );
    void setTorque( const vector3<value_type> &value );
    void setMaterialName( const std::string &value );

    [[nodiscard]]
    vector3<value_type> position() const {
        return position_;
    }

    [[nodiscard]]
    vector3<value_type> orientation() const {
        return orientation_;
    }

    [[nodiscard]]
    vector3<value_type> scale() const {
        return scale_;
    }

    [[nodiscard]]
    vector3<value_type> velocity() const {
        return velocity_;
    }

    [[nodiscard]]
    vector3<value_type> torque() const {
        return torque_;
    }

    [[nodiscard]]
    std::string materialName() const {
        return materialName_;
    };

    // Calculate transformation matrix, apply this
    // matrix to default geometry data and copy this data
    // into local buffers
    void applyTransormations( float delta );

private:
    // Default body geometry data.
    std::shared_ptr<PolytopeData> shapeData_{};

    // Buffers for local geometry data. This buffers fill
    // every frame with geometry data taken from default
    // shape data and transform according to actual spatial
    // information
    std::vector<vector3f> localVertecies_{};
    std::vector<vector3f> localNormals_{};

    BoundingVolume<value_type> bounding_{};

    // Body spatial information
    vector3<value_type> position_{};
    vector3<value_type> orientation_{};
    vector3<value_type> scale_{};
    vector3<value_type> velocity_{};
    vector3<value_type> torque_{};

    // Body material information
    Colorf albedoColor_{};
    std::string materialName_{};
};

}  // namespace tire
