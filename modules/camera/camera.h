
#pragma once

#include "algebra/vector3.h"
#include "algebra/matrix4.h"

namespace tire::camera
{

struct Camera {
    void setDirty();

    void unSetDirty();

    // recalculate projection matrix despite on _dirty flag
    algebra::matrix4d forceGetMatrix();

    algebra::matrix4d getMatrix();

    void move( const algebra::vector3d &offset );

    void rotate( double yawOffset, double pitchOffset, double rollOffset );

    void setPosition( const algebra::vector3d &pos );

    void setAngles( double yaw, double pitch, double roll );

private:
    virtual void update() = 0;

protected:
    bool dirty_{ false };

    algebra::matrix4d matrix_{};

    algebra::vector3d position_{};
    double yaw_{};
    double pitch_{};
    double roll_{};
};

struct Perspective final : Camera {
    Perspective( double fov, double aspect, double ncp, double fcp );

    void setFov( double fov );

    void setAspect( double aspect );

    void setNcp( double ncp );

    void setFcp( double fcp );

private:
    void update() override;

private:
    double fov_{};
    double aspect_{};
    double ncp_{};
    double fcp_{};
};

struct Orthographic final : Camera {
    Orthographic( double left, double right, double bottom, double top, double near, double far );

private:
    void update() override;

private:
    double left_{};
    double right_{};
    double bottom_{};
    double top_{};
    double near_{};
    double far_{};
};

} // namespace tire::camera
