
#ifndef __obj_h__
#define __obj_h__

#include <iostream>
#include <string>
#include <vector>
#include <map>

enum class ObjTokens {
    COMMENT,
    OBJECT_NAME,
    GEOMETRIC_VERTICES,
    VERTEX_NORMALS,
    TEXTURE_VERTICES,
    PARAMETER_SPACE_VERTICES,
    FORMS_OF_CURVE,
    DEGREE,
    BASIS_MATRIX,
    STEP_SIZE,
    POINT_ELEMENT,
    LINE_ELEMENT,
    FACE_ELEMENT,
    SMOOTH_GROUP,
    CURVE_ELEMENT,
    CURVE_2D_ELEMENT,
    SURFACE_ELEMENT,
};

class WavefrontObj_c {
    public:
        WavefrontObj_c(const WavefrontObj_c &rhs) = delete;
        WavefrontObj_c(const std::string &fname);
        ~WavefrontObj_c();

        WavefrontObj_c &operator=(const WavefrontObj_c &rhs) = delete;
        WavefrontObj_c &operator=(const WavefrontObj_c &&rhs) = delete;

    private:
        int vertsCount_;
        int faceCount_;

        std::vector<float> vertices_;
        std::vector<int> faceIds_;
        std::vector<float> vertNormals_;
        std::vector<float> faceTexCoords_;
        std::vector<float> vertTexCoords_;
};

#endif
