
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <string>

#include "bitmap.h"

using namespace std;

bitmap_c::bitmap_c() {
	decompressed = nullptr;
}

bitmap_c::bitmap_c(const bitmap_c &other) {
    // На случай попытки копирования неинициализорованного класса
    if (other.decompressed == nullptr) {
        decompressed = nullptr;
        return;
	};

	fname = other.fname;
	img_width = other.img_width;
	img_height = other.img_height;
	img_bpp = other.img_bpp;

	decompressed = new uint8_t [img_width*img_height*(img_bpp/8)];

    std::memcpy(reinterpret_cast<uint8_t*>(decompressed), 
                reinterpret_cast<uint8_t*>(other.decompressed), 
                img_width*img_height*(img_bpp/8)*sizeof(uint8_t));
}

bitmap_c::~bitmap_c() {
	if (decompressed != nullptr) {
		delete[] decompressed;
	}
}

//  Оператор присваивания копированием
bitmap_c& bitmap_c::operator=(const bitmap_c &other) {
    if (other.decompressed == nullptr) {
        decompressed = nullptr;
        return *this;
	};

	fname = other.fname;
	img_width = other.img_width;
	img_height = other.img_height;
	img_bpp = other.img_bpp;

	decompressed = new uint8_t [img_width*img_height*(img_bpp/8)];

    std::memcpy(reinterpret_cast<uint8_t*>(decompressed), 
                reinterpret_cast<uint8_t*>(other.decompressed), 
                img_width*img_height*(img_bpp/8)*sizeof(uint8_t));     
    // Возвращаем текущий объект, чтобы иметь возможность связать в цепочку выполнение нескольких операций присваивания
    return *this;
};

bool bitmap_c::load_tga(std::string name) {
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

bool bitmap_c::load_jpg(string name) {
	struct jpeg_error_mgr err;				//the error handler
	unsigned char * rowptr[1];				//pointer to an array
	FILE *fin;
	struct jpeg_decompress_struct info;		//for our jpeg info

	fin = fopen(name.c_str(), "rb");
	if (!fin) {
		std::cout << "bitmap_c::load_jpg(): ERROR! Unable to open file - " << name << std::endl;
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

void bitmap_c::load_dummy() {
	img_width = 512;
	img_height = 512;
	img_bpp = 24;
	
	decompressed = new uint8_t [img_width * img_height * (img_bpp/8)];
	
	for (size_t i = 0; i < img_width * img_height * (img_bpp/8); i++) {
		decompressed[i] = i % 255;
	}
}

size_t bitmap_c::get_widht() {
    return img_width;
}

size_t bitmap_c::get_height() {
    return img_height;
}

size_t bitmap_c::get_chanels_count() {
    return img_bpp / 8;
}

uint8_t* bitmap_c::get_data_ptr() {
    if (decompressed != nullptr) {
		return decompressed;
	} else {
		std::cout << "tga_img_c::get_data(): - File is empty!" << std::endl;
		return nullptr;
	}
}

void bitmap_c::show_info() {
	std::cout << "bitmap_c::show_info(): \n" 
			  << "file name - " << fname << "\n"
			  << "img_width - " << img_width << "\n"
			  << "img_height - " << img_height << "\n"
			  << "img_bpp - " << img_bpp << "\n\n";
}
