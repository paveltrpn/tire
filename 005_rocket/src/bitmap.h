#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <jpeg/jpeglib.h>    
#include <jpeg/jerror.h>
#include <tga.h>

using namespace std;

class bitmap_c {
	private:
		uint8_t* decompressed = nullptr;

		std::string fname;
        uint32_t img_width;
        uint32_t img_height;
        uint32_t img_bpp;

	public:
		bitmap_c();
		// Конструктор копирования.
        bitmap_c(const bitmap_c &other);

		~bitmap_c();

		// Оператор присваивания копированием
        bitmap_c& operator=(const bitmap_c &other);
		
		// В конце функции load_jpg() вызывается jpeg_destroy_decompress(&info)
		// которая, видимо, очищает поля структуры и поэтому не совсем
		// корректно брать инфу о файле из неё. Лучше добавить в класс отдельные поля
		// для хранения информации о изображении.
		bool load_jpg(string name);
		bool load_tga(string name);

		void load_dummy();

		size_t      get_widht();
        size_t      get_height();
        size_t      get_chanels_count();
        uint8_t*    get_data_ptr();

		void show_info();
};
