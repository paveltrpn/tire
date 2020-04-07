#pragma once

#include <iostream>
#include <string>
#include <jpeglib.h>    
#include <jerror.h>

using namespace std;

class jpeg_img_c {
	private:
		void load_dummy();
	
	public:
		int img_width, img_height;
		int img_channels;
		uint8_t *data;

		jpeg_img_c();
		~jpeg_img_c();

		bool from_file(string name);
		void show_img_stats();
		unsigned char * get_data();
};