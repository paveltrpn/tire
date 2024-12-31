
#include "scene.h"
#include "functions.h"

namespace tire::gl {

Scene::Scene( const std::filesystem::path &fname )
    : tire::Scene{ fname } {
    const auto nodeListSize = nodeList_.size();
    buffersList_.reserve( nodeListSize );
    for ( size_t i{}; i < nodeListSize; ++i ) {
        SceneNodeBufferObjects buf{};

        glGenBuffers( 1, &buf.bufferObject );
        glGenVertexArrays( 1, &buf.vertexObject );
        glBindVertexArray( buf.vertexObject );
        glBindBuffer( GL_ARRAY_BUFFER, buf.vertexObject );
        glBufferData( GL_ARRAY_BUFFER, nodeList_[i]->verteciesArraySize(),
                      nodeList_[i]->verteciesData(), GL_STATIC_DRAW );

        glEnableVertexAttribArray( 0 );
        glVertexAttribPointer( 0, 3, GL_FLOAT, false, 0, 0 );

        glGenBuffers( 1, &buf.elementsObject );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buf.elementsObject );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER,
                      nodeList_[i]->indeciesArraySize(),
                      nodeList_[i]->indeciesData(), GL_STATIC_DRAW );

        buffersList_.push_back( buf );
    }
}

void Scene::output() {
    for ( size_t i{}; i < buffersList_.size(); ++i ) {
        glBindBuffer( GL_ARRAY_BUFFER, buffersList_[i].bufferObject );

        glEnableVertexAttribArray( 0 );
        glBindVertexArray( buffersList_[i].elementsObject );
        glDrawElements( GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr );
        glDisableVertexAttribArray( 0 );

        // glBindVertexArray( buffersList_[i].vertexObject );
        // glDrawArrays( GL_TRIANGLES, 0, 3 );
    }
}

}  // namespace tire::gl
