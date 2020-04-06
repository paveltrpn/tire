
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <jpeglib.h>    
#include <jerror.h>

#include "jpeg.h"

using namespace std;

jpeg_img_c::jpeg_img_c() {
	img_width = img_height = img_channels = 0;
	data = nullptr;
}

jpeg_img_c::~jpeg_img_c() {
	if (data != nullptr) {
		delete[] data;
	}
}

bool jpeg_img_c::from_file(string fname) {
	struct jpeg_decompress_struct info;		//for our jpeg info
	struct jpeg_error_mgr err;				//the error handler
	unsigned char * rowptr[1];				//pointer to an array
	FILE *fin;
	
	fin = fopen(fname.c_str(), "rb");
	if (!fin) {
		std::cout << "ERROR! Unable to open file - " << fname << std::endl;
		std::cout << "Dummy image will be loaded!" << std::endl;
		load_dummy();
		return 1;
	}
	
	info.err = jpeg_std_error(&err);
	jpeg_create_decompress(&info); 
	
	jpeg_stdio_src(&info, fin);
	jpeg_read_header(&info, TRUE);
	
	jpeg_start_decompress(&info);

	img_width = info.output_width;
	img_height = info.output_height;
	img_channels = info.num_components;
	
	data = new uint8_t [img_width * img_height * img_channels];
	while (info.output_scanline < info.output_height) {
		rowptr[0] = (unsigned char *)data + img_channels * info.output_width * info.output_scanline; 
		jpeg_read_scanlines(&info, rowptr, 1);
	}
	
	jpeg_finish_decompress(&info);
	
	jpeg_destroy_decompress(&info);
	
	fclose(fin);
	
	return 0;
}

void jpeg_img_c::show_img_stats() {
	if (data != NULL)	{
		std::cout << "Jpeg stats for file" << std::endl; 
		std::cout << "width - " << img_width << "\n" <<
					"height - " << img_height << "\n" <<
					"channels - " << img_channels << std::endl;
	} else std::cout << "File is empty!" << std::endl;

}

unsigned char * jpeg_img_c::get_data() {
	if (data != NULL) {
		return data;
	} else {
		std::cout << "File is empty!" << std::endl;
		return NULL;
	}
}

void jpeg_img_c::load_dummy() {
	img_width = 512;
	img_height = 512;
	img_channels = 3;
	
	data = new unsigned char [img_width * img_height * img_channels];
	
	for (int i = 0; i < img_width * img_height * img_channels; i++) {
		data[i] = i % 255;
	}
}