
#include "box.h"

namespace tired {

Box::Box( vsg::dvec3 pos, vsg::dvec3 scale ) {
    auto geometry = new vsg::Geometry{};

    const auto size{ 1000.0f };
    auto verticies = vsg::dvec3Array::create( { vsg::dvec3{ -size, -size, -size }, vsg::dvec3{ size, -size, -size },
                                                vsg::dvec3{ size, size, -size }, vsg::dvec3{ -size, size, -size },
                                                vsg::dvec3{ -size, -size, size }, vsg::dvec3{ size, -size, size },
                                                vsg::dvec3{ size, size, size }, vsg::dvec3{ -size, size, size } } );

    auto indicies = vsg::uintArray::create( { 0, 1, 2, 0, 2, 3, 4, 7, 6, 4, 6, 5, 0, 4, 7, 0, 7, 3,
                                              1, 5, 6, 1, 6, 2, 0, 3, 7, 0, 7, 4, 1, 2, 6, 1, 6, 5 } );

    auto vertexDraw = vsg::VertexIndexDraw::create();
    vertexDraw->assignArrays( { verticies, indicies } );
    vertexDraw->assignIndices( indicies );
    // vertexDraw->vertexCount = 36;  // Number of indices
    vertexDraw->instanceCount = 1;

    this->addChild( vertexDraw );
}

}  // namespace tired
