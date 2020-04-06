
#pragma once

#include <iostream>
#include <string>

using namespace std;

class tga_img_c {
	private:
		uint8_t *data;

		void load_dummy();
	
	public:
		int img_width, img_height;
		int img_channels;

		tga_img_c();
		~tga_img_c();
};
