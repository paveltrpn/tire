
#ifndef __tga_h__
#define __tga_h__

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdint.h>

using namespace std;

// GCC 9.2.0 выравнивает эту структуру
// и поэтому sizeof(STGAHeader) = 20.
// Если использовать #pragma pack(push, 1)
// то sizeof(STGAHeader) = 18.
#pragma pack(push, 1)
typedef struct STGAHeader
{
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
} STGAHeader;
#pragma pack(pop)

class tga_img_c {
	private:
		STGAHeader sTGAHeader;

		uint8_t* decompressed;
	
	public:
		tga_img_c();
		~tga_img_c();

		bool from_file(string name);

		size_t      get_widht();
        size_t      get_height();
        size_t      get_chanels_count();
        uint8_t*    get_data();
};

#endif
