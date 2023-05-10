
#include <iostream>
#include <ostream>

#include "obj.h"

std::map<std::string, ObjTokens> tokens{
    { "#", ObjTokens::COMMENT },
    { "o", ObjTokens::OBJECT_NAME },
    { "v", ObjTokens::GEOMETRIC_VERTICES },
    { "vn", ObjTokens::VERTEX_NORMALS },
    { "vt", ObjTokens::TEXTURE_VERTICES },
    { "vp", ObjTokens::PARAMETER_SPACE_VERTICES },
    { "cstype", ObjTokens::FORMS_OF_CURVE },
    { "deg", ObjTokens::DEGREE },
    { "bmat", ObjTokens::BASIS_MATRIX },
    { "step", ObjTokens::STEP_SIZE },
    { "p", ObjTokens::POINT_ELEMENT },
    { "l", ObjTokens::LINE_ELEMENT },
    { "f", ObjTokens::FACE_ELEMENT },
    { "curv", ObjTokens::CURVE_ELEMENT },
    { "curv2", ObjTokens::CURVE_2D_ELEMENT },
    { "surf", ObjTokens::SURFACE_ELEMENT },
    { "s", ObjTokens::SMOOTH_GROUP },
};

WavefrontObj_c::WavefrontObj_c(const std::string &fname) {
    std::cout << "wavefront obj ctor from file " << fname << std::endl;
}

WavefrontObj_c::~WavefrontObj_c() {
}
