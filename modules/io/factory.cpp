
#include "factory.h"

#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_IO_FACTORY_CPP{ true };

namespace tire::io {

static void onOpen( uv_fs_t *req ) {
    if ( req->result < 0 ) {
        log::error( "error: {}", uv_strerror( req->result ) );
    } else {
        // uvBuf = uv_buf_init( dataBuf, sizeof( dataBuf ) );
        // uv_fs_read( uv_default_loop(), &readReq, req->result, &uvBuf, 1, -1,
        // onRead );
        log::info( "file open success" );
    }
    uv_fs_req_cleanup( req );
}

void Factory::testFileOpen( std::string_view path ) {
    static uv_fs_t openReq;
    uv_fs_open( loop_, &openReq, path.data(), O_RDONLY, 0, onOpen );
}

}  // namespace tire::io
