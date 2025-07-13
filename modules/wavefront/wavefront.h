
#pragma once

#include <fstream>
#include <string>
#include <map>
#include <vector>

#include "scene/body.h"

namespace tire {

enum class ObjTokens {
    GEOMETRIC_VERTICES,
    TEXTURE_VERTICES,
    VERTEX_NORMALS,
    PARAMETER_SPACE_VERTICES,
    CURVE_OR_SURFACE,
    DEGREE,
    BASIS_MATRIX,
    STEP_SIZE,
    POINT,
    LINE,
    FACE,
    CURVE,
    CURVE_2D,
    SOURFACE,
    PARAMETER_VALUES,
    OUTER_TRIMMING_LOOP,
    INNER_TRIMMING_LOOP,
    SPECIAL_CURVE,
    SPECIAL_POINT,
    END_STATEMENT,
    CONNECT,
    GROUP_NAME,
    SMOOTHING_GROUP,
    MERGING_GROUP,
    OBJECT_NAME,
    BEVEL_INTERPOLATION,
    COLOR_INTERPOLATION,
    DISSOLVE_INTERPOLATION,
    LEVEL_OF_DETAIL,
    MATERIAL_NAME,
    MATERIAL_LIBRARY,
    SHADOW_CASTING,
    RAY_TRACING,
    CURVE_APPROXIMATION_TECHNIQUE,
    SURFACE_APPROXIMATION_TECHNIQUE,
    COMMENT,
};

static std::map<ObjTokens, std::string> tokens_ = {
    { ObjTokens::GEOMETRIC_VERTICES, "v " },
    { ObjTokens::TEXTURE_VERTICES, "vt " },
    { ObjTokens::VERTEX_NORMALS, "vn " },
    { ObjTokens::PARAMETER_SPACE_VERTICES, "vp " },
    { ObjTokens::CURVE_OR_SURFACE, "cstype " },
    { ObjTokens::DEGREE, "deg " },
    { ObjTokens::BASIS_MATRIX, "bmat " },
    { ObjTokens::STEP_SIZE, "step " },
    { ObjTokens::POINT, "p " },
    { ObjTokens::LINE, "l " },
    { ObjTokens::FACE, "f " },
    { ObjTokens::CURVE, "curv " },
    { ObjTokens::CURVE_2D, "curv2 " },
    { ObjTokens::SOURFACE, "surf " },
    { ObjTokens::PARAMETER_VALUES, "parm " },
    { ObjTokens::OUTER_TRIMMING_LOOP, "trim " },
    { ObjTokens::INNER_TRIMMING_LOOP, "hole " },
    { ObjTokens::SPECIAL_CURVE, "scrv " },
    { ObjTokens::SPECIAL_POINT, "sp " },
    { ObjTokens::END_STATEMENT, "end " },
    { ObjTokens::CONNECT, "con " },
    { ObjTokens::GROUP_NAME, "g " },
    { ObjTokens::SMOOTHING_GROUP, "s " },
    { ObjTokens::MERGING_GROUP, "mg " },
    { ObjTokens::OBJECT_NAME, "o " },
    { ObjTokens::BEVEL_INTERPOLATION, "bevel " },
    { ObjTokens::COLOR_INTERPOLATION, "c_interp " },
    { ObjTokens::DISSOLVE_INTERPOLATION, "d_interp " },
    { ObjTokens::LEVEL_OF_DETAIL, "lod " },
    { ObjTokens::MATERIAL_NAME, "usemtl " },
    { ObjTokens::MATERIAL_LIBRARY, "mtllib " },
    { ObjTokens::SHADOW_CASTING, "shadow_obj " },
    { ObjTokens::RAY_TRACING, "trace_obj " },
    { ObjTokens::CURVE_APPROXIMATION_TECHNIQUE, "ctech " },
    { ObjTokens::SURFACE_APPROXIMATION_TECHNIQUE, "stech " },
    { ObjTokens::COMMENT, "# " },
};

struct WavefrontMtl final {
    enum class MtlTokens {};

    // using enum MtlTokens;
};

struct WavefrontObj final {
public:
    WavefrontObj() = delete;

    WavefrontObj( std::string_view path );

    ~WavefrontObj() = default;

    WavefrontObj( const WavefrontObj &rhs ) = delete;
    WavefrontObj( WavefrontObj &&rhs ) = delete;
    WavefrontObj &operator=( const WavefrontObj &rhs ) = delete;
    WavefrontObj &operator=( const WavefrontObj &&rhs ) = delete;

    [[nodiscard]] std::shared_ptr<tire::Body> getAsBody(
        std::string_view path );

    friend std::ostream &operator<<( std::ostream &os,
                                     const WavefrontObj &rhs );

private:
    void assertFilePath( std::string_view path );

    void collectObjectInfo();

private:
    std::ifstream file_;

    std::string objectName_{};
    std::string matlibFileName_{};
    size_t verteciesCount_{ 0 };
    size_t normalsCount_{ 0 };
    size_t texCoordsCount_{ 0 };
    size_t facesCount_{ 0 };
};

std::ostream &operator<<( std::ostream &os, const WavefrontObj &rhs );

}  // namespace tire
