
#pragma once

namespace tire::constants::scene {

static constexpr char PARAM_NAME_OBJECTS[] = "objects";
static constexpr char PARAM_NAME_CAMERAS[] = "cameras";
static constexpr char PARAM_NAME_LIGHTS[] = "lights";

static constexpr char PARAM_NAME_OBJECT_TYPE_BOX[] = "box";
static constexpr char PARAM_NAME_OBJECT_TYPE_FRAME[] = "frame";
static constexpr char PARAM_NAME_OBJECT_TYPE_DIMOND[] = "diamond";

static constexpr char PARAM_NAME_OBJECT_TYPE[] = "type";
static constexpr char PARAM_NAME_OBJECT_SPAWN_POS[] = "spawn_pos";
static constexpr char PARAM_NAME_OBJECT_SCALE[] = "scale";
static constexpr char PARAM_NAME_OBJECT_COLOR[] = "color";
static constexpr char PARAM_NAME_OBJECT_DIFFUSE_TEX[] = "texture_diffuse";
static constexpr char PARAM_NAME_OBJECT_USE_TORQUE[] = "use_momentum";
static constexpr char PARAM_NAME_OBJECT_TORQUE[] = "momentum";

static constexpr char PARAM_NAME_CAMERA_TYPE[] = "type";
static constexpr char PARAM_NAME_CAMERA_PERSPECTIVE[] = "perspective";
static constexpr char PARAM_NAME_CAMERA_SPAWN_POS[] = "position";
static constexpr char PARAM_NAME_CAMERA_LOOK_AT[] = "look_at";
static constexpr char PARAM_NAME_CAMERA_FOV[] = "fov";
static constexpr char PARAM_NAME_CAMERA_ASPECT[] = "aspect";
static constexpr char PARAM_NAME_CAMERA_NCP[] = "ncp";
static constexpr char PARAM_NAME_CAMERA_FCP[] = "fcp";

static constexpr char PARAM_NAME_GRAVITY_VEC[] = "gravity_vec";

}  // namespace tire::constants::scene
