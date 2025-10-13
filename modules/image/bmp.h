
#include <cstdio>
#include <fstream>
#include <cstdint>

#include "log/log.h"

namespace tire {

enum img_type_e { IMG_RGB, IMG_RGBA, IMG_BGR, IMG_BGRA };

using FXPT2DOT30 = int;

struct CIEXYZ {
    FXPT2DOT30 ciexyzX;
    FXPT2DOT30 ciexyzY;
    FXPT2DOT30 ciexyzZ;
};

struct CIEXYZTRIPLE {
    CIEXYZ ciexyzRed;
    CIEXYZ ciexyzGreen;
    CIEXYZ ciexyzBlue;
};

// bitmap file header
struct BITMAPFILEHEADER {
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
};

// bitmap info header
struct BITMAPINFOHEADER {
    unsigned int biSize;
    unsigned int biWidth;
    unsigned int biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    unsigned int biXPelsPerMeter;
    unsigned int biYPelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
    unsigned int biRedMask;
    unsigned int biGreenMask;
    unsigned int biBlueMask;
    unsigned int biAlphaMask;
    unsigned int biCSType;
    CIEXYZTRIPLE biEndpoints;
    unsigned int biGammaRed;
    unsigned int biGammaGreen;
    unsigned int biGammaBlue;
    unsigned int biIntent;
    unsigned int biProfileData;
    unsigned int biProfileSize;
    unsigned int biReserved;
};

template <typename Type>
void read( std::ifstream& fp, Type& result, std::size_t size ) {
    fp.read( reinterpret_cast<char*>( &result ), size );
}

static unsigned char bitextract( const unsigned int byte,
                                 const unsigned int mask ) {
    if ( mask == 0 ) {
        return 0;
    }

    // определение количества нулевых бит справа от маски
    unsigned int maskBufer = mask, maskPadding = 0;

    while ( !( maskBufer & 1 ) ) {
        maskBufer >>= 1;
        maskPadding++;
    }

    // применение маски и смещение
    return ( byte & mask ) >> maskPadding;
}

struct Bmp {
    Bmp( std::string_view fname );

    ~Bmp();

    size_t width();
    size_t height();
    size_t chanels();
    uint8_t* data();

private:
    BITMAPFILEHEADER fileHeader_{};
    BITMAPINFOHEADER infoHeader_{};

    uint8_t* decompressed_{ nullptr };
};

}  // namespace tire
