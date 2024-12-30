

#include <fstream>
#include <string>
#include <map>
#include <vector>

#include "wavefront.h"
#include "scene/node.h"

namespace tire {

std::vector<std::string> split( std::string_view str, char delim ) {
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

WavefrontObj::WavefrontObj( std::string_view path ) {
    assertFilePath( path );

    file_.open( path );

    if ( file_ ) {
        collectObjectInfo();
    } else {
        throw std::runtime_error( "somehow file not opened!" );
    }
}

std::shared_ptr<tire::Node> WavefrontObj::getAsNode( std::string_view path ) {
    return {};
}

void WavefrontObj::assertFilePath( std::string_view path ) {
    if ( !std::filesystem::exists( path ) ) {
        throw std::runtime_error( "file not exist!" );
    }
}

void WavefrontObj::collectObjectInfo() {
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
        } else if ( str.starts_with( tokens_[ObjTokens::VERTEX_NORMALS] ) ) {
            normalsCount_++;
            continue;
        } else if ( str.starts_with( tokens_[ObjTokens::TEXTURE_VERTICES] ) ) {
            texCoordsCount_++;
            continue;
        } else if ( str.starts_with( tokens_[ObjTokens::FACE] ) ) {
            facesCount_++;
            continue;
        } else if ( str.starts_with( tokens_[ObjTokens::MATERIAL_LIBRARY] ) ) {
            matlibFileName_ = str.erase( 0, 7 );
            continue;
        }
    }

    file_.seekg( 0, std::ios_base::beg );  // rewind
}

std::ostream &operator<<( std::ostream &os, const WavefrontObj &rhs ) {
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
