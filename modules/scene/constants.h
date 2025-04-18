
#pragma once

namespace tire::constants::scene {

static constexpr char PARAM_ENVIRONMENT[] = "environment";
static constexpr char PARAM_OBJECTS[] = "objects";
static constexpr char PARAM_CAMERAS[] = "cameras";
static constexpr char PARAM_LIGHTS[] = "lights";

static constexpr char PARAM_BACKGROUND_COLOR[] = "background_color";

static constexpr char PARAM_OBJECT_TYPE_BOX[] = "box";
static constexpr char PARAM_OBJECT_TYPE_FRAME[] = "frame";
static constexpr char PARAM_OBJECT_TYPE_DIAMOND[] = "diamond";
static constexpr char PARAM_OBJECT_TYPE_WALL01[] = "wall01";
static constexpr char PARAM_OBJECT_TYPE_ARCH01[] = "arch01";
static constexpr char PARAM_OBJECT_TYPE_PRISM[] = "prism";
static constexpr char PARAM_OBJECT_TYPE_PRISMHEXA[] = "prismhexa";
static constexpr char PARAM_OBJECT_TYPE_PYRAMIDCUT[] = "pyramidcut";

static constexpr char PARAM_OBJECT_TYPE[] = "type";

static constexpr char PARAM_OBJECT_POSITION[] = "position";
static constexpr char PARAM_OBJECT_ORIENTATION[] = "orientation";
static constexpr char PARAM_OBJECT_SCALE[] = "scale";
static constexpr char PARAM_OBJECT_VELOCITY[] = "velocity";
static constexpr char PARAM_OBJECT_TORQUE[] = "torque";

static constexpr char PARAM_OBJECT_ALBEDO_COLOR[] = "albedo_color";
static constexpr char PARAM_OBJECT_SPECULAR_COLOR[] = "specular_color";
static constexpr char PARAM_OBJECT_ALBEDO_TEXTURE[] = "albedo_texture";

static constexpr char PARAM_CAMERA_TYPE[] = "type";
static constexpr char PARAM_CAMERA_PERSPECTIVE[] = "perspective";
static constexpr char PARAM_CAMERA_EYE[] = "eye";
static constexpr char PARAM_CAMERA_TARGET[] = "target";
static constexpr char PARAM_CAMERA_AZIMUTH[] = "azimuth";
static constexpr char PARAM_CAMERA_ELEVATION[] = "elevation";
static constexpr char PARAM_CAMERA_FOV[] = "fov";
static constexpr char PARAM_CAMERA_ASPECT[] = "aspect";
static constexpr char PARAM_CAMERA_NCP[] = "ncp";
static constexpr char PARAM_CAMERA_FCP[] = "fcp";

static constexpr char PARAM_LIGHT_TYPE[] = "type";
static constexpr char PARAM_LIGHT_OMNI[] = "omni";
static constexpr char PARAM_OMNILIGHT_POSITION[] = "position";
static constexpr char PARAM_OMNILIGHT_CONSTANT[] = "constant";
static constexpr char PARAM_OMNILIGHT_LINEAR[] = "linear";
static constexpr char PARAM_OMNILIGHT_QUADRATIC[] = "quadratic";
static constexpr char PARAM_OMNILIGHT_AMBIENT[] = "ambient";
static constexpr char PARAM_OMNILIGHT_DIFFUSE[] = "diffuse";
static constexpr char PARAM_OMNILIGHT_SPECULAR[] = "specular";

static constexpr char PARAM_NAME_GRAVITY_VEC[] = "gravity_vec";

}  // namespace tire::constants::scene
