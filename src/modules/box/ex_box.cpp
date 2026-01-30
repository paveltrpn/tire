

#include "ex_box.h"

namespace tired {

ExBox::ExBox( vsg::dvec3 pos, vsg::dvec3 scale ) {
    // VK_FORMAT_R32G32B32_SFLOAT, VK_VERTEX_INPUT_RATE_INSTANCE, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE
    auto vertices = vsg::vec3Array::create( { { -0.5f, -0.5f, 0.5f },
                                              { 0.5f, -0.5f, 0.5f },
                                              { 0.5f, 0.5f, 0.5f },
                                              { -0.5f, 0.5f, 0.5f },
                                              { -0.5f, -0.5f, -0.5f },
                                              { 0.5f, -0.5f, -0.5f },
                                              { 0.5f, 0.5f, -0.5f },
                                              { -0.5f, 0.5f, -0.5f } } );

    // VK_FORMAT_R32G32B32_SFLOAT, VK_VERTEX_INPUT_RATE_VERTEX, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE
    auto colors = vsg::vec3Array::create( {
        { 1.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f },
        { 1.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f },
    } );

    // VK_FORMAT_R32G32_SFLOAT, VK_VERTEX_INPUT_RATE_VERTEX, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE
    auto texcoords = vsg::vec2Array::create( { { 0.0f, 0.0f },
                                               { 1.0f, 0.0f },
                                               { 1.0f, 1.0f },
                                               { 0.0f, 1.0f },
                                               { 0.0f, 0.0f },
                                               { 1.0f, 0.0f },
                                               { 1.0f, 1.0f },
                                               { 0.0f, 1.0f } } );

    // VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE
    //
    // TOP BOTTOM NEAR FAR LEFT RIGHT
    auto indices = vsg::uintArray::create( { 0, 1, 2, 2, 3, 0, 4, 7, 6, 6, 5, 4, 7, 3, 2, 2, 6, 7,
                                             4, 5, 1, 1, 0, 4, 3, 7, 4, 4, 0, 3, 6, 2, 1, 1, 5, 6 } );

    // setup geometry
    auto drawCommands = vsg::Commands::create();
    drawCommands->addChild( vsg::BindVertexBuffers::create( 0, vsg::DataList{ vertices, colors, texcoords } ) );
    drawCommands->addChild( vsg::BindIndexBuffer::create( indices ) );
    drawCommands->addChild( vsg::DrawIndexed::create( 36, 1, 0, 0, 0 ) );

    this->matrix = vsg::scale( scale ) * vsg::translate( pos );

    this->addChild( drawCommands );
}

}  // namespace tired
