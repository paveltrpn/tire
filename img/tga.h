
#ifndef __tga_h__
#define __tga_h__

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdint.h>

using namespace std;

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
		void load_dummy();
	
	public:
		size_t img_width, img_height;
		int img_channels;
		uint8_t *data;

		tga_img_c();
		~tga_img_c();

		bool from_file(string name);
		void show_img_stats();
};

#endif
