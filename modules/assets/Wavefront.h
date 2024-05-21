
#ifndef __assets_wavefront_h__
#define __assets_wavefront_h__

#include <fstream>
#include <string>
#include <map>

#include "geometry/Node.h"

namespace tire {

struct WavefrontMtl final {
        enum class MtlTokens {};

        using enum MtlTokens;
};

struct WavefrontObj final {
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

        using enum ObjTokens;

    public:
        WavefrontObj() = delete;

        WavefrontObj(std::string_view path) noexcept(false);

        ~WavefrontObj() = default;

        WavefrontObj(const WavefrontObj &rhs) = delete;
        WavefrontObj(WavefrontObj &&rhs) = delete;
        WavefrontObj &operator=(const WavefrontObj &rhs) = delete;
        WavefrontObj &operator=(const WavefrontObj &&rhs) = delete;

        [[nodiscard]]
        std::shared_ptr<tire::Node<float>> getAsNode(std::string_view path);

        friend std::ostream &operator<<(std::ostream &os, const WavefrontObj &rhs);

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
