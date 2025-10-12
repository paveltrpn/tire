module;

#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <format>
#include <filesystem>

#include "geometry/polytope.h"

export module wavefront:obj;

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

auto split( std::string_view str, char delim ) -> std::vector<std::string> {
    std::vector<std::string> result;
    auto left = str.begin();
    for ( auto it = left; it != str.end(); ++it ) {
        if ( *it == delim ) {
            result.emplace_back( &*left, it - left );
            left = it + 1;
        }
    }
    if ( left != str.end() ) result.emplace_back( &*left, str.end() - left );
    return result;
}

export struct Obj final {
public:
    Obj() = delete;

    Obj( const Obj& rhs ) = delete;
    Obj( Obj&& rhs ) = delete;
    Obj& operator=( const Obj& rhs ) = delete;
    Obj& operator=( const Obj&& rhs ) = delete;

    Obj( std::string_view path ) {
        assertFilePath( path );

        file_.open( std::string{ path } );

        if ( file_ ) {
            collectObjectInfo();
        } else {
            throw std::runtime_error( "somehow file not opened!" );
        }
    }

    ~Obj() = default;

    //auto getAsBody( std::string_view path ) -> std::shared_ptr<tire::Body> {
    //    return {};
    //}

    auto assertFilePath( std::string_view path ) -> void {
        if ( !std::filesystem::exists( path ) ) {
            throw std::runtime_error( "file not exist!" );
        }
    }

    auto collectObjectInfo() -> void {
        std::string str;
        while ( std::getline( file_, str ) ) {
            if ( str.starts_with( tokens_[ObjTokens::COMMENT] ) ) {
                continue;
            } else if ( str.starts_with( tokens_[ObjTokens::OBJECT_NAME] ) ) {
                objectName_ = str.erase( 0, 2 );
                continue;
            } else if ( str.starts_with(
                            tokens_[ObjTokens::GEOMETRIC_VERTICES] ) ) {
                verteciesCount_++;
                continue;
            } else if ( str.starts_with(
                            tokens_[ObjTokens::VERTEX_NORMALS] ) ) {
                normalsCount_++;
                continue;
            } else if ( str.starts_with(
                            tokens_[ObjTokens::TEXTURE_VERTICES] ) ) {
                texCoordsCount_++;
                continue;
            } else if ( str.starts_with( tokens_[ObjTokens::FACE] ) ) {
                facesCount_++;
                continue;
            } else if ( str.starts_with(
                            tokens_[ObjTokens::MATERIAL_LIBRARY] ) ) {
                matlibFileName_ = str.erase( 0, 7 );
                continue;
            }
        }

        file_.seekg( 0, std::ios_base::beg );  // rewind
    }

    friend auto operator<<( std::ostream& os, const Obj& rhs ) -> std::ostream&;

private:
    std::ifstream file_;

    std::string objectName_{};
    std::string matlibFileName_{};
    size_t verteciesCount_{ 0 };
    size_t normalsCount_{ 0 };
    size_t texCoordsCount_{ 0 };
    size_t facesCount_{ 0 };
};

export auto operator<<( std::ostream& os, const Obj& rhs ) -> std::ostream& {
    os << std::format(
        R"foo(object name: {}
material file: {}
vertecies: {}
normals: {}
texture coordinates: {}
faces: {}
)foo",
        rhs.objectName_, rhs.matlibFileName_, rhs.verteciesCount_,
        rhs.normalsCount_, rhs.texCoordsCount_, rhs.facesCount_ );
    return os;
}

}  // namespace tire
