
#ifndef __assets_wavefront_h__
#define __assets_wavefront_h__

#include <fstream>
#include <string>
#include <map>

#include "geometry/Node.h"

namespace tire {

struct WavefrontMtl final {};

struct WavefrontObj final {
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
            MATERIAL_LIB,
        };

        using enum ObjTokens;

    public:
        WavefrontObj() = delete;

        WavefrontObj(std::string_view path);

        ~WavefrontObj() = default;

        WavefrontObj(const WavefrontObj &rhs) = delete;
        WavefrontObj(WavefrontObj &&rhs) = delete;
        WavefrontObj &operator=(const WavefrontObj &rhs) = delete;
        WavefrontObj &operator=(const WavefrontObj &&rhs) = delete;

        [[nodiscard]]
        std::shared_ptr<tire::Node<float>> getAsNode(std::string_view path);

    private:
        void assertFilePath(std::string_view path) noexcept(false);
        void collectObjectInfo();

    private:
        static std::map<ObjTokens, std::string> tokens_;

        std::ifstream file_;

        std::string objectName_{};
        std::string matlibFileName_{};
        size_t verteciesCount_{ 0 };
        size_t normalsCount_{ 0 };
        size_t texCoordsCount_{ 0 };
        size_t facesCount_{ 0 };
};

}  // namespace tire

#endif
