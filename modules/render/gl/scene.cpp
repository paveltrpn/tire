
#include "scene.h"
#include "functions.h"
#include "config/config.h"

namespace tire::gl {

Scene::Scene( const std::filesystem::path &fname )
    : tire::Scene{ fname } {
    const auto nodeListSize = nodeList_.size();
    buffersList_.reserve( nodeListSize );

    for ( size_t i{}; i < nodeListSize; ++i ) {
        SceneNodeBufferObjects buf{};

        glGenVertexArrays( 1, &buf.vertexArray );
        glGenBuffers( 1, &buf.vertexBuffer );
        glGenBuffers( 1, &buf.elementsBuffer );

        glBindVertexArray( buf.vertexArray );
        glBindBuffer( GL_ARRAY_BUFFER, buf.vertexBuffer );
        glBufferData( GL_ARRAY_BUFFER, nodeList_[i]->verteciesArraySize(),
                      nodeList_[i]->verteciesData(), GL_STATIC_DRAW );

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buf.elementsBuffer );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER,
                      nodeList_[i]->indeciesArraySize(),
                      nodeList_[i]->indeciesData(), GL_STATIC_DRAW );

        // vertex positions
        glEnableVertexAttribArray( 0 );
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, nullptr );
        // vertex normals
        //glEnableVertexAttribArray(1);
        //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        //glEnableVertexAttribArray(2);
        //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        // glBindVertexArray( 0 );

        // glEnableVertexAttribArray( buf.vertexObject );
        // glVertexAttribPointer( 0, 3, GL_FLOAT, false, 0, nullptr );

        buffersList_.push_back( buf );
    }
}

void Scene::initPrograms() {
    const auto basePath = Config::instance()->getBasePath();

    colorProgram_.init<GL_VERTEX_SHADER>( basePath / "assets" / "shaders" /
                                          "color_vertex.glsl" );
    colorProgram_.init<GL_FRAGMENT_SHADER>( basePath / "assets" / "shaders" /
                                            "color_fragment.glsl" );
    colorProgram_.link();
    colorProgram_.findUniforms();
}

void Scene::submit() {
    for ( size_t i{}; i < buffersList_.size(); ++i ) {
        glBindBuffer( GL_ARRAY_BUFFER, buffersList_[i].vertexBuffer );
        void *ptr = glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );
        memcpy( ptr, nodeList_[i]->verteciesData(),
                nodeList_[i]->verteciesArraySize() );
        glUnmapBuffer( GL_ARRAY_BUFFER );
    }
}

void Scene::output() {
    colorProgram_.use();
    for ( size_t i{}; i < buffersList_.size(); ++i ) {
        // glEnableVertexAttribArray( 0 );
        // glBindBuffer( GL_ARRAY_BUFFER,
        // buffersList_[i].vertexBuffer );  // for vertex attributes
        // glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffersList_[i].elementsBuffer );
        //glBindVertexArray( buffersList_[i].vertexObject );
        // enable vertex arrays
        //glEnableClientState(GL_NORMAL_ARRAY);
        //glEnableClientState(GL_COLOR_ARRAY);
        //glEnableClientState( GL_VERTEX_ARRAY );

        // before draw, specify vertex and index arrays with their offsets
        //glNormalPointer(GL_FLOAT, 0, (void*)sizeof(vertices));
        //glColorPointer(3, GL_FLOAT, 0, (void*)(sizeof(vertices)+sizeof(normals)));
        //glVertexPointer( 3, GL_FLOAT, 0, 0 );

        colorProgram_.setColor( nodeList_[i]->color() );
        colorProgram_.setViewMatrix( getCamera( 0 )->getMatrix() );

        glBindVertexArray( buffersList_[i].vertexArray );
        glDrawElements( GL_TRIANGLES, nodeList_[i]->indeciesCount(),
                        GL_UNSIGNED_INT, nullptr );
        glBindVertexArray( 0 );
        // glDisableVertexAttribArray( 0 );
        // glBindBuffer( GL_ARRAY_BUFFER, 0 );
        // glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    }
}

}  // namespace tire::gl
