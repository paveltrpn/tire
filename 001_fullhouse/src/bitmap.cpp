
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <string>
#include <jpeglib.h>    
#include <jerror.h>

#include "bitmap.h"

using namespace std;

jpeg_img_c::jpeg_img_c() {
	decompressed = nullptr;
}

jpeg_img_c::~jpeg_img_c() {
	if (decompressed != nullptr) {
		delete[] decompressed;
	}
}

bool jpeg_img_c::from_file(string name) {
	struct jpeg_error_mgr err;				//the error handler
	unsigned char * rowptr[1];				//pointer to an array
	FILE *fin;
	
	fin = fopen(name.c_str(), "rb");
	if (!fin) {
		std::cout << "jpeg_img_c::from_file(): ERROR! Unable to open file - " << name << std::endl;
		return false;
	}
	
	info.err = jpeg_std_error(&err);
	jpeg_create_decompress(&info); 
	
	jpeg_stdio_src(&info, fin);
	jpeg_read_header(&info, TRUE);
	
	jpeg_start_decompress(&info);

	fname = name;
    img_width = info.output_width;
    img_height = info.output_height;
    img_bpp = info.num_components*8;

	decompressed = new uint8_t [info.output_width * info.output_height * info.num_components];

	while (info.output_scanline < info.output_height) {
		rowptr[0] = (unsigned char *)decompressed + info.num_components * info.output_width * info.output_scanline; 
		jpeg_read_scanlines(&info, rowptr, 1);
	}
	
	jpeg_finish_decompress(&info);
	
	jpeg_destroy_decompress(&info);
	
	fclose(fin);
	
	return 0;
}

size_t jpeg_img_c::get_widht() {
    return img_width;
}

size_t jpeg_img_c::get_height() {
    return img_height;
}

size_t jpeg_img_c::get_chanels_count() {
    return img_bpp;
}

uint8_t * jpeg_img_c::get_data() {
	if (decompressed != nullptr) {
		return decompressed;
	} else {
		std::cout << "jpeg_img_c::get_data(): - File is empty!" << std::endl;
		return nullptr;
	}
}

void jpeg_img_c::show_img_stats() {
	std::cout << "jpeg_img_c::show_img_stats(): file name - " << fname << "; img_width - " << img_width <<
				"; img_height - " << img_height << "; img_bpp - " << img_bpp << ";\n";
}

/*
void jpeg_img_c::load_dummy() {
	img_width = 512;
	img_height = 512;
	img_channels = 3;
	
	data = new unsigned char [img_width * img_height * img_channels];
	
	for (int i = 0; i < img_width * img_height * img_channels; i++) {
		data[i] = i % 255;
	}
}
*/

tga_img_c::tga_img_c() {
	decompressed = nullptr;
}

tga_img_c::~tga_img_c() {
	if (decompressed != nullptr) {
		delete[] decompressed;
	}
}

bool tga_img_c::from_file(std::string name) {
    TGA *in = TGAOpen(name.c_str(), "rb");
	TGAData in_data;

	std::memset(&in_data, 0, sizeof(TGAData));
	in_data.flags = TGA_IMAGE_ID | TGA_IMAGE_DATA | TGA_RGB;

	TGAReadImage(in, &in_data);
	
	fname = name;
    img_width = in->hdr.width;
    img_height = in->hdr.height;
    img_bpp = in->hdr.depth;

    decompressed = new uint8_t [img_width*img_height*(img_bpp/8)];

    std::memcpy(decompressed, static_cast<uint8_t*>(in_data.img_data), img_width*img_height*(img_bpp/8));

	TGAFreeTGAData(&in_data);
	TGAClose(in);

	return 0;
}

size_t tga_img_c::get_widht() {
    return img_width;
}

size_t tga_img_c::get_height() {
    return img_height;
}

size_t tga_img_c::get_chanels_count() {
    return img_bpp / 8;
}

uint8_t* tga_img_c::get_data() {
    if (decompressed != nullptr) {
		return decompressed;
	} else {
		std::cout << "tga_img_c::get_data(): - File is empty!" << std::endl;
		return nullptr;
	}
}

void tga_img_c::show_img_stats() {
	std::cout << "tga_img_c::show_img_stats(): file name - " << fname << "; img_width - " << img_width <<
				"; img_height - " << img_height << "; img_bpp - " << img_bpp << "\n";
}
