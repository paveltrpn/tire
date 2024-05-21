
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

#include "bmp.h"

using namespace std;

unsigned char bitextract(const unsigned int byte, const unsigned int mask) {
    if (mask == 0) {
        return 0;
    }

    // определение количества нулевых бит справа от маски
    unsigned int maskBufer = mask, maskPadding = 0;

    while (!(maskBufer & 1)) {
        maskBufer >>= 1;
        maskPadding++;
    }

    // применение маски и смещение
    return (byte & mask) >> maskPadding;
}

bmp_img_c::bmp_img_c() {
    decompressed = nullptr;
}

bmp_img_c::~bmp_img_c() {
    if (decompressed != nullptr) {
        delete[] decompressed;
    }
}

bool bmp_img_c::from_file(string fname) {
    std::ifstream fileStream(fname, std::ifstream::binary);
    if (!fileStream) {
        std::cout << "Error opening file '" << fname << "." << std::endl;
        return 0;
    }

    // заголовк изображения
    read(fileStream, file_header.bfType, sizeof(file_header.bfType));
    read(fileStream, file_header.bfSize, sizeof(file_header.bfSize));
    read(fileStream, file_header.bfReserved1, sizeof(file_header.bfReserved1));
    read(fileStream, file_header.bfReserved2, sizeof(file_header.bfReserved2));
    read(fileStream, file_header.bfOffBits, sizeof(file_header.bfOffBits));

    if (file_header.bfType != 0x4D42) {
        std::cout << "bmp_img_c::from_file(): Error -'" << fname << "' is not BMP file."
                  << std::endl;
        return 0;
    }

    // информация изображения
    read(fileStream, info_header.biSize, sizeof(info_header.biSize));

    // bmp core
    if (info_header.biSize >= 12) {
        read(fileStream, info_header.biWidth, sizeof(info_header.biWidth));
        read(fileStream, info_header.biHeight, sizeof(info_header.biHeight));
        read(fileStream, info_header.biPlanes, sizeof(info_header.biPlanes));
        read(fileStream, info_header.biBitCount, sizeof(info_header.biBitCount));
    }

    // получаем информацию о битности
    int colorsCount = info_header.biBitCount >> 3;
    if (colorsCount < 3) {
        colorsCount = 3;
    }

    int bitsOnColor = info_header.biBitCount / colorsCount;
    int maskValue = (1 << bitsOnColor) - 1;

    // bmp v1
    if (info_header.biSize >= 40) {
        read(fileStream, info_header.biCompression, sizeof(info_header.biCompression));
        read(fileStream, info_header.biSizeImage, sizeof(info_header.biSizeImage));
        read(fileStream, info_header.biXPelsPerMeter, sizeof(info_header.biXPelsPerMeter));
        read(fileStream, info_header.biYPelsPerMeter, sizeof(info_header.biYPelsPerMeter));
        read(fileStream, info_header.biClrUsed, sizeof(info_header.biClrUsed));
        read(fileStream, info_header.biClrImportant, sizeof(info_header.biClrImportant));
    }

    // bmp v2
    info_header.biRedMask = 0;
    info_header.biGreenMask = 0;
    info_header.biBlueMask = 0;

    if (info_header.biSize >= 52) {
        read(fileStream, info_header.biRedMask, sizeof(info_header.biRedMask));
        read(fileStream, info_header.biGreenMask, sizeof(info_header.biGreenMask));
        read(fileStream, info_header.biBlueMask, sizeof(info_header.biBlueMask));
    }

    // если маска не задана, то ставим маску по умолчанию
    if (info_header.biRedMask == 0 || info_header.biGreenMask == 0 || info_header.biBlueMask == 0) {
        info_header.biRedMask = maskValue << (bitsOnColor * 2);
        info_header.biGreenMask = maskValue << bitsOnColor;
        info_header.biBlueMask = maskValue;
    }

    // bmp v3
    if (info_header.biSize >= 56) {
        read(fileStream, info_header.biAlphaMask, sizeof(info_header.biAlphaMask));
    } else {
        info_header.biAlphaMask = maskValue << (bitsOnColor * 3);
    }

    // bmp v4
    if (info_header.biSize >= 108) {
        read(fileStream, info_header.biCSType, sizeof(info_header.biCSType));
        read(fileStream, info_header.biEndpoints, sizeof(info_header.biEndpoints));
        read(fileStream, info_header.biGammaRed, sizeof(info_header.biGammaRed));
        read(fileStream, info_header.biGammaGreen, sizeof(info_header.biGammaGreen));
        read(fileStream, info_header.biGammaBlue, sizeof(info_header.biGammaBlue));
    }

    // bmp v5
    if (info_header.biSize >= 124) {
        read(fileStream, info_header.biIntent, sizeof(info_header.biIntent));
        read(fileStream, info_header.biProfileData, sizeof(info_header.biProfileData));
        read(fileStream, info_header.biProfileSize, sizeof(info_header.biProfileSize));
        read(fileStream, info_header.biReserved, sizeof(info_header.biReserved));
    }

    // проверка на поддерку этой версии формата
    if (info_header.biSize != 12 && info_header.biSize != 40 && info_header.biSize != 52
        && info_header.biSize != 56 && info_header.biSize != 108 && info_header.biSize != 124) {
        std::cout << "bmp_img_c::from_file(): Error - Unsupported BMP format." << std::endl;
        return 0;
    }

    if (info_header.biBitCount != 16 && info_header.biBitCount != 24
        && info_header.biBitCount != 32) {
        std::cout << "bmp_img_c::from_file(): Error - Unsupported BMP bit count." << std::endl;
        return 0;
    }

    if (info_header.biCompression != 0 && info_header.biCompression != 3) {
        std::cout << "bmp_img_c::from_file(): Error - Unsupported BMP compression." << std::endl;
        return 0;
    }

    // определение размера отступа в конце каждой строки
    int linePadding = ((info_header.biWidth * (info_header.biBitCount / 8)) % 4) & 3;

    uint32_t bufer;
    size_t i, j;

    /*  Чтение файла в массив uint8_t *decompressed */
    decompressed = new uint8_t[info_header.biHeight * info_header.biWidth * 4];

    for (i = 0; i < info_header.biHeight; i++) {
        for (j = 0; j < info_header.biWidth; j++) {
            read(fileStream, bufer, info_header.biBitCount / 8);

            decompressed[((i * 4) * info_header.biHeight + j * 4) + 2]
              = bitextract(bufer, info_header.biRedMask);
            decompressed[((i * 4) * info_header.biHeight + j * 4) + 1]
              = bitextract(bufer, info_header.biGreenMask);
            decompressed[((i * 4) * info_header.biHeight + j * 4) + 0]
              = bitextract(bufer, info_header.biBlueMask);
            decompressed[((i * 4) * info_header.biHeight + j * 4) + 3]
              = bitextract(bufer, info_header.biAlphaMask);
        }
        fileStream.seekg(linePadding, std::ios_base::cur);
    }

    fileStream.close();

    return false;
}

size_t bmp_img_c::get_widht() {
    return info_header.biWidth;
}

size_t bmp_img_c::get_height() {
    return info_header.biHeight;
}

size_t bmp_img_c::get_chanels_count() {
    return info_header.biBitCount / 8;
}

uint8_t* bmp_img_c::get_data() {
    return decompressed;
}
