
#ifndef __load_jpeg_h__
#define __load_jpeg_h__

#include <cstdio>
#include <fmt/format.h>
#include <fstream>
#include <iostream>
#include <string>

#include "jerror.h"
#include "jpeglib.h"
#include "tga.h"

enum img_type_e { IMG_RGB, IMG_RGBA, IMG_BGR, IMG_BGRA };

typedef int FXPT2DOT30;

typedef struct {
        FXPT2DOT30 ciexyzX;
        FXPT2DOT30 ciexyzY;
        FXPT2DOT30 ciexyzZ;
} CIEXYZ;

typedef struct {
        CIEXYZ ciexyzRed;
        CIEXYZ ciexyzGreen;
        CIEXYZ ciexyzBlue;
} CIEXYZTRIPLE;

// bitmap file header
typedef struct {
        unsigned short bfType;
        unsigned int bfSize;
        unsigned short bfReserved1;
        unsigned short bfReserved2;
        unsigned int bfOffBits;
} BITMAPFILEHEADER;

// bitmap info header
typedef struct {
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
} BITMAPINFOHEADER;

template <typename Type> void read(std::ifstream& fp, Type& result, std::size_t size) {
    fp.read(reinterpret_cast<char*>(&result), size);
}

unsigned char bitextract(const unsigned int byte, const unsigned int mask);

class CBitmap {
    private:
        uint8_t* decompressed;

        uint32_t bitmapWidth;
        uint32_t bitmapHeight;
        uint32_t bitmapChannels;

        std::string bitmapName;  //

        bool readFromTgaFile(const std::string& fname);
        bool readFromJpegFile(const std::string& fname);
        bool readFromBmpFile(const std::string& fname);

    public:
        CBitmap() {
            decompressed = nullptr;
        };

        ~CBitmap() {
            if (decompressed != nullptr) {
                delete[] decompressed;
            }
        };

        void loadDummyCheckerPattern(uint32_t texSize = 512,
                                     uint32_t channelsCount = 3,
                                     uint32_t checkerSquareSizePx = 64);

        bool readFromFile(const std::string& fname);
        bool writeToJpegFile(const std::string& fname);

        size_t getWidht();
        size_t getHeight();
        size_t getChanelsCount();
        uint8_t* getDataPtr();
};

#endif