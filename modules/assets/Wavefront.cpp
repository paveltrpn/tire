
#include <iostream>
#include <fstream>
#include <filesystem>
#include <stdexcept>

#include "Wavefront.h"

namespace tire {

std::map<WavefrontObj::ObjTokens, std::string> WavefrontObj::tokens_{
    { GEOMETRIC_VERTICES, "v " },
    { TEXTURE_VERTICES, "vt " },
    { VERTEX_NORMALS, "vn " },
    { PARAMETER_SPACE_VERTICES, "vp " },
    { CURVE_OR_SURFACE, "cstype " },
    { DEGREE, "deg " },
    { BASIS_MATRIX, "bmat " },
    { STEP_SIZE, "step " },
    { POINT, "p " },
    { LINE, "l " },
    { FACE, "f " },
    { CURVE, "curv " },
    { CURVE_2D, "curv2 " },
    { SOURFACE, "surf " },
    { PARAMETER_VALUES, "parm " },
    { OUTER_TRIMMING_LOOP, "trim " },
    { INNER_TRIMMING_LOOP, "hole " },
    { SPECIAL_CURVE, "scrv " },
    { SPECIAL_POINT, "sp " },
    { END_STATEMENT, "end " },
    { CONNECT, "con " },
    { GROUP_NAME, "g " },
    { SMOOTHING_GROUP, "s " },
    { MERGING_GROUP, "mg " },
    { OBJECT_NAME, "o " },
    { BEVEL_INTERPOLATION, "bevel " },
    { COLOR_INTERPOLATION, "c_interp " },
    { DISSOLVE_INTERPOLATION, "d_interp " },
    { LEVEL_OF_DETAIL, "lod " },
    { MATERIAL_NAME, "usemtl " },
    { MATERIAL_LIBRARY, "mtllib " },
    { SHADOW_CASTING, "shadow_obj " },
    { RAY_TRACING, "trace_obj " },
    { CURVE_APPROXIMATION_TECHNIQUE, "ctech " },
    { SURFACE_APPROXIMATION_TECHNIQUE, "stech " },
    { COMMENT, "# " },
};

std::vector<std::string> split(std::string_view str, char delim) {
    std::vector<std::string> result;
    auto left = str.begin();
    for (auto it = left; it != str.end(); ++it) {
        if (*it == delim) {
            result.emplace_back(&*left, it - left);
            left = it + 1;
        }
    }
    if (left != str.end())
        result.emplace_back(&*left, str.end() - left);
    return result;
}

std::ostream &operator<<(std::ostream &os, const WavefrontObj &rhs) {
    os << std::format(
      R"foo(object name: {}
material file: {}
vertecies: {}
normals: {}
texture coordinates: {}
faces: {}
)foo",
      rhs.objectName_,
      rhs.matlibFileName_,
      rhs.verteciesCount_,
      rhs.normalsCount_,
      rhs.texCoordsCount_,
      rhs.facesCount_);
    return os;
}

WavefrontObj::WavefrontObj(std::string_view path) {
    assertFilePath(path);

    file_.open(path);

    if (file_) {
        collectObjectInfo();
    } else {
        throw std::runtime_error("somehow file not opened!");
    }
}

void WavefrontObj::assertFilePath(std::string_view path) {
    if (!std::filesystem::exists(path)) {
        throw std::runtime_error("file not exist!");
    }
}

void WavefrontObj::collectObjectInfo() {
    std::string str;
    while (std::getline(file_, str)) {
        if (str.starts_with(tokens_[COMMENT])) {
            continue;
        } else if (str.starts_with(tokens_[OBJECT_NAME])) {
            objectName_ = str.erase(0, 2);
            continue;
        } else if (str.starts_with(tokens_[GEOMETRIC_VERTICES])) {
            verteciesCount_++;
            continue;
        } else if (str.starts_with(tokens_[VERTEX_NORMALS])) {
            normalsCount_++;
            continue;
        } else if (str.starts_with(tokens_[TEXTURE_VERTICES])) {
            texCoordsCount_++;
            continue;
        } else if (str.starts_with(tokens_[FACE])) {
            facesCount_++;
            continue;
        } else if (str.starts_with(tokens_[MATERIAL_LIBRARY])) {
            matlibFileName_ = str.erase(0, 7);
            continue;
        }
    }

    file_.seekg(0, std::ios_base::beg);  // rewind
}

std::shared_ptr<tire::Node<float>> WavefrontObj::getAsNode(std::string_view path) {
    return {};
}

}  // namespace tire
