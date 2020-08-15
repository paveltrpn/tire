#pragma once

#include <iostream>
#include <string>
#include <jpeglib.h>    
#include <jerror.h>
#include <tga.h>

using namespace std;

class jpeg_img_c {
	private:
		// В конце функции from_file() вызывается jpeg_destroy_decompress(&info)
		// которая, видимо, очищает поля структуры и поэтому не совсем
		// корректно брать инфу о файле из неё. Лучше добавить в класс отдельные поля
		// для хранения информации о изображении.
		struct jpeg_decompress_struct info;		//for our jpeg info

		uint8_t* decompressed;
	
		std::string fname;
        uint32_t img_width;
        uint32_t img_height;
        uint32_t img_bpp;
        
	public:
	
		jpeg_img_c();
		~jpeg_img_c();

		bool from_file(string name);

		size_t      get_widht();
        size_t      get_height();
        size_t      get_chanels_count();
        uint8_t*    get_data();

		void show_img_stats();
};

class tga_img_c {
	private:
		uint8_t* decompressed;

		std::string fname;
        uint32_t img_width;
        uint32_t img_height;
        uint32_t img_bpp;
	
	public:
		tga_img_c();
		~tga_img_c();

		bool from_file(string name);

		size_t      get_widht();
        size_t      get_height();
        size_t      get_chanels_count();
        uint8_t*    get_data();

		void show_img_stats();
};
