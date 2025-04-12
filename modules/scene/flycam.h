#pragma once

#include <type_traits>
#include "algebra/vector3.h"
#include "algebra/matrix4.h"

#include "algebra/vector3.h"
#include "geometry/bounding_volumes.h"

#include "render_tag.h"

namespace tire {

struct Flycam final {
    Flycam() = default;

    Flycam( const algebra::vector3f &eye, const algebra::vector3f &target );

    Flycam( const algebra::vector3f &eye, float yaw, float pitch, float roll );

    Flycam( const Flycam &other ) = delete;
    Flycam( Flycam &&other ) = delete;

    Flycam &operator=( const Flycam &other ) = delete;
    Flycam &operator=( Flycam &&other ) = delete;

    void setFov( float fov );
    void setAspect( float aspect );
    void setNcp( float ncp );
    void setFcp( float fcp );

    void move( const algebra::vector3f &offset );
    void rotate( float yawOffset, float pitchOffset, float rollOffset );
    void setPosition( const algebra::vector3f &pos );
    void setAngles( float yaw, float pitch, float roll );

    template <RenderType Type>
    algebra::matrix4f matrix() {
        algebra::matrix4f projection;
        // Choose projection matrix according to render type
        if constexpr ( std::is_same_v<Type, OpenGLTag> ) {
            projection =
                algebra::perspective<float>( fov_, aspect_, ncp_, fcp_ );
        } else if constexpr ( std::is_same_v<Type, VulkanTag> ) {
            projection =
                algebra::vperspective<float>( fov_, aspect_, ncp_, fcp_ );
        }
        auto offset = algebra::translate<float>( eye_ );
        auto rotation = algebra::rotate<float>( yaw_, pitch_, roll_ );

        offset.transposeSelf();
        return offset * rotation * projection;

        // algebra::matrix4f lookAt{};
        // lookAt.lookAt( algebra::vector3f{ 0.0, 0.0, 0.0 },
        //    algebra::vector3f{ 0.0, 0.0, 1.0 },
        //    algebra::vector3f{ 0.0, 1.0, 0.0 } );
        // matrix_ = lookAt * projection;
    }

private:
    float fov_{};
    float aspect_{};
    float ncp_{};
    float fcp_{};

    algebra::vector3f eye_{ 0.0f, 0.0f, 0.0f };

    float yaw_{};
    float pitch_{};
    float roll_{};

    BoundingSphere<float> bounding_{};
    float mass_{};
    algebra::vector3f velocity_{};
};

}  // namespace tire