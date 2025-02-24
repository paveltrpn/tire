
#pragma once

#include "algebra/vector3.h"
#include "algebra/matrix4.h"

namespace tire {

struct Camera {
    algebra::matrix4f matrix();

    void move( const algebra::vector3f &offset );
    void rotate( float yawOffset, float pitchOffset, float rollOffset );
    void setPosition( const algebra::vector3f &pos );
    void setAngles( float yaw, float pitch, float roll );

private:
    virtual void update() = 0;

protected:
    algebra::matrix4f matrix_{};
    algebra::vector3f position_{};
    float yaw_{};
    float pitch_{};
    float roll_{};
};

struct Perspective final : Camera {
    Perspective( float fov, float aspect, float ncp, float fcp );

    void setFov( float fov );
    void setAspect( float aspect );
    void setNcp( float ncp );
    void setFcp( float fcp );

private:
    void update() override;

private:
    float fov_{};
    float aspect_{};
    float ncp_{};
    float fcp_{};
};

struct Orthographic final : Camera {
    Orthographic( float left, float right, float bottom, float top, float near,
                  float far );

private:
    void update() override;

private:
    float left_{};
    float right_{};
    float bottom_{};
    float top_{};
    float near_{};
    float far_{};
};

}  // namespace tire
