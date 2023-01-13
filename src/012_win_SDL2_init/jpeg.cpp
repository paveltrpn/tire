
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include "jpeglib.h"    
#include "jerror.h"

#include "jpeg.h"

using namespace std;

jpeg_img_c::jpeg_img_c() {
	decompressed = nullptr;
}

jpeg_img_c::~jpeg_img_c() {
	if (decompressed != nullptr) {
		delete[] decompressed;
	}
}

bool jpeg_img_c::from_file(string fname) {
	struct jpeg_error_mgr err;				//the error handler
	unsigned char * rowptr[1];				//pointer to an array
	FILE *fin;
	
	fin = fopen(fname.c_str(), "rb");
	if (!fin) {
		std::cout << "jpeg_img_c::from_file(): ERROR! Unable to open file - " << fname << std::endl;
		return false;
	}
	
	info.err = jpeg_std_error(&err);
	jpeg_create_decompress(&info); 
	
	jpeg_stdio_src(&info, fin);
	jpeg_read_header(&info, TRUE);
	
	jpeg_start_decompress(&info);

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
    return info.output_width;
}

size_t jpeg_img_c::get_height() {
    return info.output_height;
}

size_t jpeg_img_c::get_chanels_count() {
    return info.num_components;
}

uint8_t * jpeg_img_c::get_data() {
	if (decompressed != nullptr) {
		return decompressed;
	} else {
		std::cout << "File is empty!" << std::endl;
		return nullptr;
	}
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