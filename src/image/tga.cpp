
#include <fstream>
#include <format>
#include <iostream>
#include <fstream>

#include "tga.h"
#include "log/log.h"

namespace tired {

Tga::Tga( std::string_view fname ) {
    uint8_t color[4] = { 0, 0, 0, 255 };
    uint8_t blockInfo{}, isPacked{};
    uint32_t numPixels{};
    size_t i{}, k{};

    // Open file.
    std::ifstream _fileStream;
    _fileStream.open( std::string{ fname }, std::ifstream::binary );
    if ( !_fileStream ) {
        throw std::runtime_error( std::format( "tga file \"{}\" not found", std::string{ fname } ) );
    }

    // Read header.
    std::array<std::byte, TGA_HEADER_SIZE> header{};
    _fileStream.read( reinterpret_cast<char*>( &header ), TGA_HEADER_SIZE );
    _header.identsize = static_cast<uint8_t>( header[0] );
    _header.colorMapType = static_cast<uint8_t>( header[1] );
    _header.imageType = static_cast<uint8_t>( header[2] );
    _header.colorMapStart = static_cast<uint8_t>( header[3] ) + static_cast<uint16_t>( header[4] ) * 256;
    _header.colorMapLength = static_cast<uint8_t>( header[5] ) + static_cast<uint16_t>( header[6] ) * 256;
    _header.colorMapBits = static_cast<uint8_t>( header[7] );
    _header.xstart = static_cast<uint8_t>( header[8] ) + static_cast<uint16_t>( header[9] ) * 256;
    _header.ystart = static_cast<uint8_t>( header[10] ) + static_cast<uint16_t>( header[11] ) * 256;
    _header.width = static_cast<uint8_t>( header[12] ) + static_cast<uint16_t>( header[13] ) * 256;
    _header.height = static_cast<uint8_t>( header[14] ) + static_cast<uint16_t>( header[15] ) * 256;
    _header.bits = static_cast<uint8_t>( header[16] );
    _header.descriptor = static_cast<uint8_t>( header[17] );

    if ( ( _header.bits != 24 ) && ( _header.bits != 32 ) ) {
        throw std::runtime_error(
            std::format( "error read tga file it must be RGB or RGBA but have {} bpp!", _header.bits ) );
    }

    if ( _header.imageType == TGA_IMGTYPE_BW_FLAG ) {
        log::info()( "note that tga image is grayscale!" );
    }

    // Read file body.
    _width = _header.width;
    _height = _header.height;

    // Actual image format.
    const auto availableComponents = _header.bits / 8;

    // Always expand RGB to RGBA image.
    if ( _header.bits == 24 ) {
        _bpp = _header.bits + 8;
    } else {
        _bpp = _header.bits;
    }

    _components = _bpp / 8;

    _data = new uint8_t[_header.width * _header.height * _components];

    auto getByte = [&_fileStream, availableComponents]( uint8_t *c ) {
        // availableComponents can be == 3 or 4. Hence we read 4 bytes in 4-bytes array
        // or read 3 bytes and leave 4-th byte unused.
        _fileStream.read( reinterpret_cast<char *>( c ), availableComponents );
    };

    auto setByte = [this]( size_t i, uint8_t c[4] ) {
        // TGA store pixel as BGRA, write it to decompressed
        // array as RGBA.
        _data[i * _components + 0] = c[2];
        _data[i * _components + 1] = c[1];
        _data[i * _components + 2] = c[0];
        _data[i * _components + 3] = c[3];
    };

    // Read TGA as non compressed data.
    if ( _header.imageType == static_cast<decltype( _header.imageType )>( TgaCompress::NONE ) ) {
        for ( i = 0; i < _header.width * _header.height; i++ ) {
            getByte( color );
            setByte( i, color );
        }
    }

    // Read RLE compressed TGA.
    if ( _header.imageType == static_cast<decltype( _header.imageType )>( TgaCompress::RLE ) ) {
        for ( i = 0; i < _header.width * _header.height; ) {
            _fileStream.read( reinterpret_cast<char *>( &blockInfo ), 1 );

            isPacked = blockInfo & 128;
            numPixels = blockInfo & 127;

            if ( isPacked ) {
                getByte( color );

                for ( k = 0; k < numPixels + 1; ++k ) {
                    setByte( i, color );
                    ++i;
                }
            } else {
                for ( k = 0; k < numPixels + 1; ++k ) {
                    getByte( color );
                    setByte( i, color );
                    ++i;
                }
            }
        }
    }

    _fileStream.close();
}

}  // namespace tired
