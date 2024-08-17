
module;

export module camera;

import toy_std;

namespace tire::camera {

export {
    struct Camera {
            using scalar_type = float;
            using matrix4_type = toy::algebra::matrix4<scalar_type>;
            using vector3_type = toy::algebra::vector3<scalar_type>;

            void setDirty() {
                dirty_ = true;
            };

            void unSetDirty() {
                dirty_ = false;
            }

            // recalculate projection matrix despite on _dirty flag
            matrix4_type forceGetMatrix() {
                update();
                unSetDirty();
                return matrix_;
            }

            matrix4_type getMatrix() {
                if (!dirty_) {
                    return matrix_;
                } else {
                    return forceGetMatrix();
                }
            }

            void move(const vector3_type& offset) {
                position_.plus(offset);
                setDirty();
            }

            void rotate(scalar_type yawOffset, scalar_type pitchOffset, scalar_type rollOffset) {
                yaw_ += yawOffset;
                pitch_ += pitchOffset;
                roll_ += rollOffset;
                setDirty();
            }

            void setPosition(const toy::algebra::vector3<scalar_type>& pos) {
                position_ = pos;
                setDirty();
            }

            void setAngles(scalar_type yaw, scalar_type pitch, scalar_type roll) {
                yaw_ = yaw;
                pitch_ = pitch;
                roll_ = roll;
                setDirty();
            }

        private:
            virtual void update() = 0;

        protected:
            bool dirty_{ false };

            matrix4_type matrix_{};

            vector3_type position_{};
            scalar_type yaw_{};
            scalar_type pitch_{};
            scalar_type roll_{};
    };

    struct Perspective final : Camera {
            Perspective(scalar_type fov, scalar_type aspect, scalar_type ncp, scalar_type fcp)
                : fov_{ fov }, aspect_{ aspect }, ncp_{ ncp }, fcp_{ fcp } {
                update();
                unSetDirty();
            }

            void setFov(scalar_type fov) {
                fov_ = fov;
                setDirty();
            }

            void setAspect(scalar_type aspect) {
                aspect_ = aspect;
                setDirty();
            }

            void setNcp(scalar_type ncp) {
                ncp_ = ncp;
                setDirty();
            }

            void setFcp(scalar_type fcp) {
                fcp_ = fcp;
                setDirty();
            }

        private:
            void update() override {
                auto projection = toy::algebra::perspective<scalar_type>(fov_, aspect_, ncp_, fcp_);
                auto offset = toy::algebra::translate(position_);
                auto rotation = toy::algebra::rotate(yaw_, pitch_, roll_);

                matrix_ = projection * rotation * toy::algebra::transpose(offset);
            }

        private:
            scalar_type fov_{};
            scalar_type aspect_{};
            scalar_type ncp_{};
            scalar_type fcp_{};
    };

    struct Orthographic final : Camera {
            Orthographic(scalar_type left,
                         scalar_type right,
                         scalar_type bottom,
                         scalar_type top,
                         scalar_type near,
                         scalar_type far)
                : left_{ left },
                  right_{ right },
                  bottom_{ bottom },
                  top_{ top },
                  near_{ near },
                  far_{ far } {
                update();
                unSetDirty();
            }

        private:
            void update() override {
                auto projection
                  = toy::algebra::orthographic(left_, right_, bottom_, top_, near_, far_);
                auto offset = toy::algebra::translate(position_);
                auto rotation = toy::algebra::rotate(yaw_, pitch_, roll_);

                matrix_ = projection * rotation * toy::algebra::transpose(offset);
            }

        private:
            scalar_type left_{};
            scalar_type right_{};
            scalar_type bottom_{};
            scalar_type top_{};
            scalar_type near_{};
            scalar_type far_{};
    };
}
}  // namespace tire::camera
