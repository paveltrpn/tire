
module;

#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <format>
#include <filesystem>
#include <memory>

export module scene:obj;

import :mesh;

import algebra;

using namespace tire::algebra;

namespace tire {

export {
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

    std::map<ObjTokens, std::string> tokens_ = {
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
        std::vector<std::string> result{};
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

    [[nodiscard]]
    auto readObjFile( const std::string &filePath ) -> std::shared_ptr<Mesh> {
        // Assert file exist.
        if ( !std::filesystem::exists( filePath ) ) {
            throw std::runtime_error( "file not exist!" );
        }

        // Open file.
        std::ifstream fileHandle;
        fileHandle.open( filePath );
        if ( fileHandle ) {
            throw std::runtime_error( "somehow file not opened!" );
        }

        // Value to return.
        auto mesh = new Mesh{};

        // Buffers to read in.
        std::vector<vector3<Mesh::value_type>> vertices_;
        std::vector<int> indices_;
        std::vector<vector3<Mesh::value_type>> normals_;
        std::vector<vector2<Mesh::value_type>> texcrds_;
        std::vector<vector3<Mesh::value_type>> vertclr_;

        // Read file content
        std::string str;
        while ( std::getline( fileHandle, str ) ) {
            if ( str.starts_with( tokens_[ObjTokens::COMMENT] ) ) {
                continue;
            } else if ( str.starts_with( tokens_[ObjTokens::OBJECT_NAME] ) ) {
                mesh->setName( str.erase( 0, 2 ) );
                continue;
            } else if ( str.starts_with( tokens_[ObjTokens::GEOMETRIC_VERTICES] ) ) {
                //verteciesCount_++;
                continue;
            } else if ( str.starts_with( tokens_[ObjTokens::VERTEX_NORMALS] ) ) {
                //normalsCount_++;
                continue;
            } else if ( str.starts_with( tokens_[ObjTokens::TEXTURE_VERTICES] ) ) {
                //texCoordsCount_++;
                continue;
            } else if ( str.starts_with( tokens_[ObjTokens::FACE] ) ) {
                //facesCount_++;
                continue;
            } else if ( str.starts_with( tokens_[ObjTokens::MATERIAL_LIBRARY] ) ) {
                //matlibFileName_ = str.erase( 0, 7 );
                continue;
            }
        }

        return std::shared_ptr<Mesh>( mesh );
    }
}

}  // namespace tire
