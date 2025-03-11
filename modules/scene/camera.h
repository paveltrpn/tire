
#pragma once

#include <type_traits>
#include "algebra/vector3.h"
#include "algebra/matrix4.h"

namespace tire {

struct OpenGLTag {};
struct VulkanTag {};

// Concept for compile time branch or overload by
// render type.
template <typename T>
concept RenderType =
    std::is_same_v<T, OpenGLTag> || std::is_same_v<T, VulkanTag>;

struct Camera {
    void move( const algebra::vector3f &offset );
    void rotate( float yawOffset, float pitchOffset, float rollOffset );
    void setPosition( const algebra::vector3f &pos );
    void setAngles( float yaw, float pitch, float roll );

protected:
    algebra::matrix4f matrix_{};
    algebra::vector3f position_{};
    float yaw_{};
    float pitch_{};
    float roll_{};
};

struct Perspective final : Camera {
    Perspective( float fov, float aspect, float ncp, float fcp );

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
        auto offset = algebra::translate<float>( position_ );
        const auto rotation = algebra::rotate<float>( yaw_, pitch_, roll_ );

        offset.transposeSelf();
        matrix_ = offset * rotation * projection;

        // algebra::matrix4f lookAt{};
        // lookAt.lookAt( algebra::vector3f{ 0.0, 0.0, 10.0 },
        //    algebra::vector3f{ 2.4, -2.2, -3.0 },
        //    algebra::vector3f{ 0.0, 1.0, 0.0 } );
        // matrix_ = lookAt * projection;

        return matrix_;
    }

    void setFov( float fov );
    void setAspect( float aspect );
    void setNcp( float ncp );
    void setFcp( float fcp );

private:
    float fov_{};
    float aspect_{};
    float ncp_{};
    float fcp_{};
};

}  // namespace tire
