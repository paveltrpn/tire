
module;

#include <string>
#include <cstdio>
#include <fstream>
#include <iostream>

#include "spdlog/spdlog.h"

export module image.bmp;

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
void read(std::ifstream &fp, Type &result, std::size_t size) {
    fp.read(reinterpret_cast<char *>(&result), size);
}

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

export {
    struct bmp {
            bmp(std::string_view fname) {
                std::ifstream fileStream(fname, std::ifstream::binary);
                if (!fileStream) {
                    std::cout << "Error opening file '" << fname << "." << std::endl;
                    // return 0;
                }

                // заголовк изображения
                read(fileStream, fileHeader_.bfType, sizeof(fileHeader_.bfType));
                read(fileStream, fileHeader_.bfSize, sizeof(fileHeader_.bfSize));
                read(fileStream, fileHeader_.bfReserved1, sizeof(fileHeader_.bfReserved1));
                read(fileStream, fileHeader_.bfReserved2, sizeof(fileHeader_.bfReserved2));
                read(fileStream, fileHeader_.bfOffBits, sizeof(fileHeader_.bfOffBits));

                if (fileHeader_.bfType != 0x4D42) {
                    std::cout << "bmp_img_c::from_file(): Error -'" << fname << "' is not BMP file."
                              << std::endl;
                    // return 0;
                }

                // информация изображения
                read(fileStream, infoHeader_.biSize, sizeof(infoHeader_.biSize));

                // bmp core
                if (infoHeader_.biSize >= 12) {
                    read(fileStream, infoHeader_.biWidth, sizeof(infoHeader_.biWidth));
                    read(fileStream, infoHeader_.biHeight, sizeof(infoHeader_.biHeight));
                    read(fileStream, infoHeader_.biPlanes, sizeof(infoHeader_.biPlanes));
                    read(fileStream, infoHeader_.biBitCount, sizeof(infoHeader_.biBitCount));
                }

                // получаем информацию о битности
                int colorsCount = infoHeader_.biBitCount >> 3;
                if (colorsCount < 3) {
                    colorsCount = 3;
                }

                int bitsOnColor = infoHeader_.biBitCount / colorsCount;
                int maskValue = (1 << bitsOnColor) - 1;

                // bmp v1
                if (infoHeader_.biSize >= 40) {
                    read(fileStream, infoHeader_.biCompression, sizeof(infoHeader_.biCompression));
                    read(fileStream, infoHeader_.biSizeImage, sizeof(infoHeader_.biSizeImage));
                    read(
                      fileStream, infoHeader_.biXPelsPerMeter, sizeof(infoHeader_.biXPelsPerMeter));
                    read(
                      fileStream, infoHeader_.biYPelsPerMeter, sizeof(infoHeader_.biYPelsPerMeter));
                    read(fileStream, infoHeader_.biClrUsed, sizeof(infoHeader_.biClrUsed));
                    read(
                      fileStream, infoHeader_.biClrImportant, sizeof(infoHeader_.biClrImportant));
                }

                // bmp v2
                infoHeader_.biRedMask = 0;
                infoHeader_.biGreenMask = 0;
                infoHeader_.biBlueMask = 0;

                if (infoHeader_.biSize >= 52) {
                    read(fileStream, infoHeader_.biRedMask, sizeof(infoHeader_.biRedMask));
                    read(fileStream, infoHeader_.biGreenMask, sizeof(infoHeader_.biGreenMask));
                    read(fileStream, infoHeader_.biBlueMask, sizeof(infoHeader_.biBlueMask));
                }

                // если маска не задана, то ставим маску по умолчанию
                if (infoHeader_.biRedMask == 0 || infoHeader_.biGreenMask == 0
                    || infoHeader_.biBlueMask == 0) {
                    infoHeader_.biRedMask = maskValue << (bitsOnColor * 2);
                    infoHeader_.biGreenMask = maskValue << bitsOnColor;
                    infoHeader_.biBlueMask = maskValue;
                }

                // bmp v3
                if (infoHeader_.biSize >= 56) {
                    read(fileStream, infoHeader_.biAlphaMask, sizeof(infoHeader_.biAlphaMask));
                } else {
                    infoHeader_.biAlphaMask = maskValue << (bitsOnColor * 3);
                }

                // bmp v4
                if (infoHeader_.biSize >= 108) {
                    read(fileStream, infoHeader_.biCSType, sizeof(infoHeader_.biCSType));
                    read(fileStream, infoHeader_.biEndpoints, sizeof(infoHeader_.biEndpoints));
                    read(fileStream, infoHeader_.biGammaRed, sizeof(infoHeader_.biGammaRed));
                    read(fileStream, infoHeader_.biGammaGreen, sizeof(infoHeader_.biGammaGreen));
                    read(fileStream, infoHeader_.biGammaBlue, sizeof(infoHeader_.biGammaBlue));
                }

                // bmp v5
                if (infoHeader_.biSize >= 124) {
                    read(fileStream, infoHeader_.biIntent, sizeof(infoHeader_.biIntent));
                    read(fileStream, infoHeader_.biProfileData, sizeof(infoHeader_.biProfileData));
                    read(fileStream, infoHeader_.biProfileSize, sizeof(infoHeader_.biProfileSize));
                    read(fileStream, infoHeader_.biReserved, sizeof(infoHeader_.biReserved));
                }

                // проверка на поддерку этой версии формата
                if (infoHeader_.biSize != 12 && infoHeader_.biSize != 40 && infoHeader_.biSize != 52
                    && infoHeader_.biSize != 56 && infoHeader_.biSize != 108
                    && infoHeader_.biSize != 124) {
                    std::cout << "bmp_img_c::from_file(): Error - Unsupported BMP format."
                              << std::endl;
                    // return 0;
                }

                if (infoHeader_.biBitCount != 16 && infoHeader_.biBitCount != 24
                    && infoHeader_.biBitCount != 32) {
                    std::cout << "bmp_img_c::from_file(): Error - Unsupported BMP bit count."
                              << std::endl;
                    // return 0;
                }

                if (infoHeader_.biCompression != 0 && infoHeader_.biCompression != 3) {
                    std::cout << "bmp_img_c::from_file(): Error - Unsupported BMP compression."
                              << std::endl;
                    // return 0;
                }

                // определение размера отступа в конце каждой строки
                int linePadding = ((infoHeader_.biWidth * (infoHeader_.biBitCount / 8)) % 4) & 3;

                uint32_t bufer;
                size_t i, j;

                /*  Чтение файла в массив uint8_t *decompressed_ */
                decompressed_ = new uint8_t[infoHeader_.biHeight * infoHeader_.biWidth * 4];

                for (i = 0; i < infoHeader_.biHeight; i++) {
                    for (j = 0; j < infoHeader_.biWidth; j++) {
                        read(fileStream, bufer, infoHeader_.biBitCount / 8);

                        decompressed_[((i * 4) * infoHeader_.biHeight + j * 4) + 2]
                          = bitextract(bufer, infoHeader_.biRedMask);
                        decompressed_[((i * 4) * infoHeader_.biHeight + j * 4) + 1]
                          = bitextract(bufer, infoHeader_.biGreenMask);
                        decompressed_[((i * 4) * infoHeader_.biHeight + j * 4) + 0]
                          = bitextract(bufer, infoHeader_.biBlueMask);
                        decompressed_[((i * 4) * infoHeader_.biHeight + j * 4) + 3]
                          = bitextract(bufer, infoHeader_.biAlphaMask);
                    }
                    fileStream.seekg(linePadding, std::ios_base::cur);
                }

                fileStream.close();
            }

            ~bmp() {
                if (decompressed_ != nullptr) {
                    delete[] decompressed_;
                }
            };

            size_t width() {
                return infoHeader_.biWidth;
            }

            size_t height() {
                return infoHeader_.biHeight;
            }

            size_t chanels() {
                return infoHeader_.biBitCount / 8;
            }

            uint8_t *data() {
                return decompressed_;
            }

        private:
            BITMAPFILEHEADER fileHeader_{};
            BITMAPINFOHEADER infoHeader_{};

            uint8_t *decompressed_{ nullptr };
    };
}

}  // namespace tire
