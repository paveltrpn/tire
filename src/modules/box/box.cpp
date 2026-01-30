
#include "box.h"

namespace tired {

Box::Box( vsg::dvec3 pos, vsg::dvec3 scale ) {
    auto geometry = new vsg::Geometry{};

    const auto half{ 0.5f };
    // auto verticies = vsg::dvec3Array::create( { vsg::dvec3{ -size, -size, -size }, vsg::dvec3{ size, -size, -size },
    // vsg::dvec3{ size, size, -size }, vsg::dvec3{ -size, size, -size },
    // vsg::dvec3{ -size, -size, size }, vsg::dvec3{ size, -size, size },
    // vsg::dvec3{ size, size, size }, vsg::dvec3{ -size, size, size } } );

    auto verticies = vsg::vec3Array::create(
        { vsg::vec3( -half, -half, -half ), vsg::vec3( half, -half, -half ), vsg::vec3( half, half, -half ),
          vsg::vec3( -half, -half, -half ), vsg::vec3( half, half, -half ),  vsg::vec3( -half, half, -half ),
          vsg::vec3( -half, -half, half ),  vsg::vec3( -half, half, half ),  vsg::vec3( half, half, half ),
          vsg::vec3( -half, -half, half ),  vsg::vec3( half, half, half ),   vsg::vec3( half, -half, half ),
          vsg::vec3( -half, -half, -half ), vsg::vec3( -half, -half, half ), vsg::vec3( -half, half, half ),
          vsg::vec3( -half, -half, -half ), vsg::vec3( -half, half, half ),  vsg::vec3( -half, half, -half ),
          vsg::vec3( half, -half, -half ),  vsg::vec3( half, half, -half ),  vsg::vec3( half, half, half ),
          vsg::vec3( half, -half, -half ),  vsg::vec3( half, half, half ),   vsg::vec3( half, -half, half ),
          vsg::vec3( -half, -half, -half ), vsg::vec3( half, -half, -half ), vsg::vec3( half, -half, half ),
          vsg::vec3( -half, -half, -half ), vsg::vec3( half, -half, half ),  vsg::vec3( -half, -half, half ),
          vsg::vec3( -half, half, -half ),  vsg::vec3( half, half, -half ),  vsg::vec3( half, half, half ),
          vsg::vec3( -half, half, -half ),  vsg::vec3( half, half, half ),   vsg::vec3( -half, half, half ) } );

    auto indicies = vsg::uintArray::create( { 0, 1, 2, 0, 2, 3, 4, 7, 6, 4, 6, 5, 0, 4, 7, 0, 7, 3,
                                              1, 5, 6, 1, 6, 2, 0, 3, 7, 0, 7, 4, 1, 2, 6, 1, 6, 5 } );

    auto vertexDraw = vsg::VertexDraw::create();
    vertexDraw->assignArrays( { verticies } );
    // vertexDraw->assignIndices( );
    vertexDraw->vertexCount = 36;  // Number of indices
    vertexDraw->instanceCount = 1;

    this->matrix = vsg::scale( scale ) * vsg::translate( pos );

    this->addChild( vertexDraw );
}

}  // namespace tired
