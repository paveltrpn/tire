
#include <fstream>
#include <iostream>
#include <cstdint>
#include <cstdio>
#include <string>

#include "log/log.h"

namespace tire {
// GCC 9.2.0 выравнивает эту структуру
// и поэтому sizeof(STGAHeader) = 20.
// Если использовать #pragma pack(push, 1)
// то sizeof(STGAHeader) = 18, как и должно
#pragma pack( push, 1 )
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
    uint8_t bits;
    uint8_t descriptor;
};
#pragma pack( pop )

struct Tga {
    Tga( std::string_view fname );

    ~Tga();

    size_t widht();
    size_t height();
    size_t chanels();
    uint8_t *data();

private:
    STGAHeader sTGAHeader_{};

    uint8_t *decompressed_{ nullptr };
};

/*
        Взято из репозитория https://github.com/da-nie/SoftwareGraphicsLibrary/
        Статья на хабре - https://habr.com/ru/post/495664/

static const uint32_t TGA_IMAGE_TYPE_NO_IMAGE = 0;
static const uint32_t TGA_IMAGE_TYPE_PALETTE = 1;
static const uint32_t TGA_IMAGE_TYPE_RGB = 2;
static const uint32_t TGA_IMAGE_TYPE_GRAYSCALE = 3;
static const uint32_t TGA_IMAGE_TYPE_MASK = (1 << 2) | (1 << 1) | (1 << 0);
static const uint32_t TGA_IMAGE_TYPE_RLE_MASK = (1 << 3);

static const uint32_t TGA_DESCRIPTOR_FORWARD = 32;
static const uint32_t TGA_DESCRIPTOR_REVERSE = 8;

static const uint32_t TGA_COLOR_MAP_NO_PALETTE = 0;
static const uint32_t TGA_COLOR_MAP_PALETTE = 1;

    bool tga_img_c::__from_file(string fname) {
        std::ifstream fileStream(fname, std::ifstream::binary);
        if (!fileStream) {
            std::cout << "tga_img_c::from_file(): Error opening file '" << fname << "." <<
    std::endl; return 0;
        }

        // определяем размер файла
        fileStream.seekg(0,std::ios_base::end);
        uint32_t file_size = static_cast<uint32_t>(fileStream.tellg());
        fileStream.seekg(0,std::ios_base::beg);

        uint8_t *data_ptr = new uint8_t[file_size+1];

        //  читаем весь файл разом в временною переменную uint8_t *data_ptr
        fileStream.read(reinterpret_cast<char*>(data_ptr), sizeof(uint8_t)*file_size);

        fileStream.close();

        STGAHeader sTGAHeader_;

        // если размер считанного файла меньше
        // размера заголовка tga
        if (file_size < sizeof(STGAHeader)) {
            return false;
        }

        memcpy(&sTGAHeader_,data_ptr,sizeof(STGAHeader));

        if (sTGAHeader_.imageType&TGA_IMAGE_TYPE_RLE_MASK) {
            return false;//RLE
        }

        if ((sTGAHeader_.imageType&TGA_IMAGE_TYPE_MASK)==TGA_IMAGE_TYPE_NO_IMAGE ||
    (sTGAHeader_.imageType&TGA_IMAGE_TYPE_MASK)==TGA_IMAGE_TYPE_GRAYSCALE) return(NULL);//ãðàäàöèè
    ñåðîãî è îòñóòñòâèå èçîáðàæåíèÿ íå ïîääåðæèâàåì

        img_width = sTGAHeader_.width;
        img_height = sTGAHeader_.height;
        img_channels = sTGAHeader_.bits/8;

        int32_t line_length=sTGAHeader_.width*sizeof(uint32_t);

        int32_t image_length=sTGAHeader_.width*sTGAHeader_.height*sTGAHeader_.bits/8;

        uint32_t image_offset=sizeof(struct
    STGAHeader)+sTGAHeader_.colorMapStart+sTGAHeader_.colorMapLength*sTGAHeader_.colorMapBits/8+sTGAHeader_.identsize;
        if (image_offset+image_length > file_size) return(NULL);

        if (sTGAHeader_.bits==24) { //BGR
            data=new uint8_t[sTGAHeader_.width*sTGAHeader_.height*sizeof(uint32_t)];
            int32_t y,x;
            if (sTGAHeader_.descriptor==TGA_DESCRIPTOR_FORWARD) {
                uint8_t *oi_ptr=data;
                uint8_t *i_ptr=data_ptr+image_offset;
                for(y=0;y<sTGAHeader_.height;y++,i_ptr+=sTGAHeader_.width*3) {
                    uint8_t *i_ptrc=i_ptr;
                    for(x=0;x<sTGAHeader_.width;x++) {
                        uint8_t b=*(i_ptrc);i_ptrc++;
                        uint8_t g=*(i_ptrc);i_ptrc++;
                        uint8_t r=*(i_ptrc);i_ptrc++;
                        uint8_t a=1;
                        *oi_ptr=b;oi_ptr++;
                        *oi_ptr=g;oi_ptr++;
                        *oi_ptr=r;oi_ptr++;
                        *oi_ptr=a;oi_ptr++;
                    }
                }
            }

            if (sTGAHeader_.descriptor==TGA_DESCRIPTOR_REVERSE) {
                uint8_t *oi_ptr=data;
                uint8_t *i_ptr=data_ptr+image_offset+sTGAHeader_.width*sTGAHeader_.height*3-1;

                for(y=sTGAHeader_.height-1;y>=0;y--,i_ptr-=sTGAHeader_.width*3) {
                    uint8_t *i_ptrc=i_ptr;

                    for(x=0;x<sTGAHeader_.width;x++) {
                        uint8_t b=*(i_ptrc);i_ptrc++;
                        uint8_t g=*(i_ptrc);i_ptrc++;
                        uint8_t r=*(i_ptrc);i_ptrc++;
                        uint8_t a=1;
                        *oi_ptr=b;oi_ptr++;
                        *oi_ptr=g;oi_ptr++;
                        *oi_ptr=r;oi_ptr++;
                        *oi_ptr=a;oi_ptr++;
                    }
                }
            }
        }

        if (sTGAHeader_.bits==32) {
            data=new uint8_t[sTGAHeader_.width*sTGAHeader_.height*sizeof(uint32_t)];
            int32_t y,x;

            if (sTGAHeader_.descriptor==TGA_DESCRIPTOR_FORWARD) {
                uint8_t *oi_ptr=data;
                uint8_t *i_ptr=data_ptr+image_offset;

                for(y=0;y<sTGAHeader_.height;y++,i_ptr+=sTGAHeader_.width*sizeof(uint32_t)) {
                    uint8_t *i_ptrc=i_ptr;

                    for(x=0;x<sTGAHeader_.width;x++) {
                        uint8_t b=*(i_ptrc);i_ptrc++;
                        uint8_t g=*(i_ptrc);i_ptrc++;
                        uint8_t r=*(i_ptrc);i_ptrc++;
                        uint8_t a=*(i_ptrc);i_ptrc++;
                        *oi_ptr=b;oi_ptr++;
                        *oi_ptr=g;oi_ptr++;
                        *oi_ptr=r;oi_ptr++;
                        *oi_ptr=a;oi_ptr++;
                    }
                }
            }

            if (sTGAHeader_.descriptor==TGA_DESCRIPTOR_REVERSE) {
                uint8_t *oi_ptr=data;
                uint8_t *i_ptr=data_ptr+image_offset+sTGAHeader_.width*sTGAHeader_.height*3-1;

                for(y=sTGAHeader_.height-1;y>=0;y--,i_ptr-=sTGAHeader_.width*sizeof(uint32_t)) {
                    uint8_t *i_ptrc=i_ptr;

                    for(x=0;x<sTGAHeader_.width;x++) {
                        uint8_t b=*(i_ptrc);i_ptrc++;
                        uint8_t g=*(i_ptrc);i_ptrc++;
                        uint8_t r=*(i_ptrc);i_ptrc++;
                        uint8_t a=*(i_ptrc);i_ptrc++;
                        *oi_ptr=b;oi_ptr++;
                        *oi_ptr=g;oi_ptr++;
                        *oi_ptr=r;oi_ptr++;
                        *oi_ptr=a;oi_ptr++;
                    }
                }
            }
        }

        if (sTGAHeader_.colorMapType==TGA_COLOR_MAP_PALETTE && sTGAHeader_.bits/8==3) {
            uint32_t
    palette_offset=sizeof(STGAHeader)+sTGAHeader_.colorMapStart+sTGAHeader_.identsize;

            if (palette_offset+sTGAHeader_.colorMapLength*3 > file_size) return(NULL);

            uint8_t *color_map=data_ptr+palette_offset;

            data=new uint8_t[sTGAHeader_.width*sTGAHeader_.height*sizeof(uint32_t)];
            int32_t y,x;

            if (sTGAHeader_.descriptor==TGA_DESCRIPTOR_FORWARD) {
                uint8_t *oi_ptr=data;
                uint8_t *i_ptr=data_ptr+image_offset;

                for(y=0;y<sTGAHeader_.height;y++,i_ptr+=sTGAHeader_.width) {
                    uint8_t *i_ptrc=i_ptr;

                    for(x=0;x<sTGAHeader_.width;x++,i_ptrc++) {
                        int32_t index=(*i_ptrc)*3;
                        uint8_t b=color_map[index];
                        uint8_t g=color_map[index+1];
                        uint8_t r=color_map[index+2];
                        uint8_t a=1;
                        *oi_ptr=b;oi_ptr++;
                        *oi_ptr=g;oi_ptr++;
                        *oi_ptr=r;oi_ptr++;
                        *oi_ptr=a;oi_ptr++;
                    }
                }
            }

            if (sTGAHeader_.descriptor==TGA_DESCRIPTOR_REVERSE) {
                uint8_t *oi_ptr=data;
                uint8_t *i_ptr=data_ptr+image_offset+sTGAHeader_.width*(sTGAHeader_.height-1);

                for(y=sTGAHeader_.height-1;y>=0;y--,i_ptr-=sTGAHeader_.width) {
                    uint8_t *i_ptrc=i_ptr;

                    for(x=0;x<sTGAHeader_.width;x++,i_ptrc++) {
                        int32_t index=(*i_ptrc)*3;
                        uint8_t b=color_map[index];
                        uint8_t g=color_map[index+1];
                        uint8_t r=color_map[index+2];
                        uint8_t a=1;
                        *oi_ptr=b;oi_ptr++;
                        *oi_ptr=g;oi_ptr++;
                        *oi_ptr=r;oi_ptr++;
                        *oi_ptr=a;oi_ptr++;
                    }
                }
            }
        }

        return true;
    }
    */
}  // namespace tire
