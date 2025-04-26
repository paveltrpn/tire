
#pragma once

#include <fstream>
#include <iostream>
#include <cstdint>
#include <cstdio>
#include <string>

#include "log/log.h"

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
    // NITE: bit per pixel, not channels count.
    uint8_t bits;
    uint8_t descriptor;
};

struct Tga {
    Tga( std::string_view fname );

    ~Tga();

    size_t widht() const;
    size_t height() const;
    size_t channels() const;
    uint8_t *data() const;

private:
    void openFile( std::string_view fname );
    void readHeader();

private:
    STGAHeader _header{};
    uint8_t *_decompressed{ nullptr };

    std::ifstream _fileStream;
};

}  // namespace tire
