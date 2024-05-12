
#include <iostream>
#include <fstream>
#include <filesystem>
#include <stdexcept>

#include "Wavefront.h"

namespace tire {

std::map<WavefrontObj::ObjTokens, std::string> WavefrontObj::tokens_{
    { COMMENT, "#" },
    { OBJECT_NAME, "o" },
    { GEOMETRIC_VERTICES, "v" },
    { VERTEX_NORMALS, "vn" },
    { TEXTURE_VERTICES, "vt" },
    { PARAMETER_SPACE_VERTICES, "vp" },
    { FORMS_OF_CURVE, "cstype" },
    { DEGREE, "deg" },
    { BASIS_MATRIX, "bmat" },
    { STEP_SIZE, "step" },
    { POINT_ELEMENT, "p" },
    { LINE_ELEMENT, "l" },
    { FACE_ELEMENT, "f" },
    { CURVE_ELEMENT, "curv" },
    { CURVE_2D_ELEMENT, "curv2" },
    { SURFACE_ELEMENT, "surf" },
    { SMOOTH_GROUP, "s" },
    { MATERIAL_LIB, "mtllib" },
};

WavefrontObj::WavefrontObj(std::string_view path) {
    assertFilePath(path);

    file_.open(path);

    if (file_) {
        collectObjectInfo();
    }
}

void WavefrontObj::assertFilePath(std::string_view path) {
    if (std::filesystem::exists(path)) {
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
        } else if (str.starts_with(tokens_[FACE_ELEMENT])) {
            facesCount_++;
            continue;
        } else if (str.starts_with(tokens_[MATERIAL_LIB])) {
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
