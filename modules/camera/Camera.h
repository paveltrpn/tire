
#ifndef __tire_camera_h__
#define __tire_camera_h__

#include "algebra/Matrix.h"
#include "algebra/Vector.h"

namespace tire {

struct Camera {
        using scalar_type = float;
        using matrix4_type = tire::algebra::Matrix4<scalar_type>;
        using vector3_type = tire::algebra::Vector3<scalar_type>;

        void setDirty() {
            dirty_ = true;
        };

        void unSetDirty() {
            dirty_ = false;
        }

        // recalculate projection matrix despite on _dirty flag
        matrix4_type forceGetMatrix() {
            // TODO recalculate projection_
            unSetDirty();
            return projection_;
        }

        matrix4_type getMatrix() {
            if (!dirty_) {
                // auto offsetMatrix = tire::algebra::
                return projection_;
            } else {
                return forceGetMatrix();
            }
        }

        void perspective(float fov, float aspect, float ncp, float fcp) {
            fov_ = fov;
            aspect_ = aspect;
            ncp_ = ncp;
            fcp_ = fcp;

            projection_.perspective(fov, aspect, ncp, fcp);
            setDirty();
        }

        void move(const vector3_type& offset) {
            position_.sum(offset);
            setDirty();
        }

        void rotate(scalar_type yawOffset, scalar_type pitchOffset, scalar_type rollOffset) {
            yaw_ += yawOffset;
            pitch_ += pitchOffset;
            roll_ += rollOffset;
            setDirty();
        }

    private:
        bool dirty_{ false };

        scalar_type fov_{};
        scalar_type aspect_{};
        scalar_type ncp_{};
        scalar_type fcp_{};

        matrix4_type projection_{};

        vector3_type position_{};
        scalar_type yaw_{};
        scalar_type pitch_{};
        scalar_type roll_{};
};

}  // namespace tire
#endif
