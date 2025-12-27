module;

#include <cstdint>
#include <fstream>
#include <iostream>
#include <format>
#include <fstream>

export module image:tga;

import log;
import :image;

namespace tire {

#define TGA_HEADER_SIZE 18

/* sections */
#define TGA_IMAGE_ID 0x01
#define TGA_IMAGE_INFO 0x02
#define TGA_IMAGE_DATA 0x04
#define TGA_COLOR_MAP 0x08

/* color format */
#define TGA_RGB 0x20
#define TGA_BGR 0x40

/* orientation */
#define TGA_BOTTOM 0x0
#define TGA_TOP 0x1
#define TGA_LEFT 0x0
#define TGA_RIGHT 0x1

/* image types */
#define TGA_IMGTYPE_NOIMAGE 0
#define TGA_IMGTYPE_UNCOMP_CMAP 1
#define TGA_IMGTYPE_UNCOMP_TRUEC 2
#define TGA_IMGTYPE_UNCOMP_BW 3
#define TGA_IMGTYPE_RLE_CMAP 9
#define TGA_IMGTYPE_RLE_TRUEC 10
#define TGA_IMGTYPE_RLE_BW 11

#define TGA_IMGTYPE_CMAP_FLAG 0x1
#define TGA_IMGTYPE_TRUEC_FLAG 0x2
#define TGA_IMGTYPE_BW_FLAG 0x3

enum class TgaCompress { NONE = 2, RLE = 10 };

struct STGAHeader {
    uint8_t identsize;
    uint8_t colorMapType;
    uint8_t imageType;
    uint16_t colorMapStart;
    uint16_t colorMapLength;
    uint8_t colorMapBits;
    uint16_t xstart;
    uint16_t ystart;
    uint16_t width;
    uint16_t height;
    // NOTE: bit per pixel, not channels count.
    uint8_t bits;
    uint8_t descriptor;
};

export struct Tga final : Image {
    Tga( std::string_view fname ) {
        uint8_t color[4] = { 0, 0, 0, 0 };
        uint8_t blockInfo{}, isPacked{};
        uint32_t numPixels{};
        size_t i{}, k{};

        // Open file.
        std::ifstream _fileStream;
        _fileStream.open( std::string{ fname }, std::ifstream::binary );
        if ( !_fileStream ) {
            throw std::runtime_error( std::format( "tga file \"{}\" not found",
                                                   std::string{ fname } ) );
        }

        // Read header.
        std::array<std::byte, TGA_HEADER_SIZE> header{};
        _fileStream.read( reinterpret_cast<char*>( &header ), TGA_HEADER_SIZE );
        _header.identsize = static_cast<uint8_t>( header[0] );
        _header.colorMapType = static_cast<uint8_t>( header[1] );
        _header.imageType = static_cast<uint8_t>( header[2] );
        _header.colorMapStart = static_cast<uint8_t>( header[3] ) +
                                static_cast<uint16_t>( header[4] ) * 256;
        _header.colorMapLength = static_cast<uint8_t>( header[5] ) +
                                 static_cast<uint16_t>( header[6] ) * 256;
        _header.colorMapBits = static_cast<uint8_t>( header[7] );
        _header.xstart = static_cast<uint8_t>( header[8] ) +
                         static_cast<uint16_t>( header[9] ) * 256;
        _header.ystart = static_cast<uint8_t>( header[10] ) +
                         static_cast<uint16_t>( header[11] ) * 256;
        _header.width = static_cast<uint8_t>( header[12] ) +
                        static_cast<uint16_t>( header[13] ) * 256;
        _header.height = static_cast<uint8_t>( header[14] ) +
                         static_cast<uint16_t>( header[15] ) * 256;
        _header.bits = static_cast<uint8_t>( header[16] );
        _header.descriptor = static_cast<uint8_t>( header[7] );

        if ( ( _header.bits != 24 ) && ( _header.bits != 32 ) ) {
            throw std::runtime_error( std::format(
                "error read tga file it must be RGB or RGBA but have {} bpp!",
                _header.bits ) );
        }

        if ( _header.imageType == TGA_IMGTYPE_BW_FLAG ) {
            log::info( "note that tga image is grayscale!" );
        }

        // Read file body.

        bpp_ = _header.bits;
        width_ = _header.width;
        height_ = _header.height;

        const auto channels = _header.bits / 8;

        data_ = new uint8_t[_header.width * _header.height * channels];

        auto getByte = [&_fileStream, channels]( uint8_t* c ) {
            // channels can be == 3 or 4. Hence we read 4 bytes in 4-bytes array
            // or read 3 bytes and leave 4-th byte unused.
            _fileStream.read( reinterpret_cast<char*>( c ), channels );
        };

        auto setByte = [this, channels]( size_t i, uint8_t c[4] ) {
            // TGA store pixel as BGR, write it to decompressed
            // array as RGB.
            data_[i * channels + 0] = c[2];
            data_[i * channels + 1] = c[1];
            data_[i * channels + 2] = c[0];

            if ( channels == 4 ) {
                data_[i * channels + 3] = c[3];
            }
        };

        // Read TGA as non compressed data.
        if ( _header.imageType ==
             static_cast<decltype( _header.imageType )>( TgaCompress::NONE ) ) {
            for ( i = 0; i < _header.width * _header.height; i++ ) {
                getByte( color );
                setByte( i, color );
            }
        }

        // Read RLE compressed TGA.
        if ( _header.imageType ==
             static_cast<decltype( _header.imageType )>( TgaCompress::RLE ) ) {
            for ( i = 0; i < _header.width * _header.height; ) {
                _fileStream.read( reinterpret_cast<char*>( &blockInfo ), 1 );

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

private:
    STGAHeader _header{};
};

}  // namespace tire
