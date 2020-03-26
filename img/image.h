
#pragma once

#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <jpeglib.h>    
#include <jerror.h>

using namespace std;

enum img_type_e {IMG_RGB, IMG_RGBA, IMG_BGR, IMG_BGRA};

typedef int FXPT2DOT30;
 
typedef struct {
    FXPT2DOT30 ciexyzX;
    FXPT2DOT30 ciexyzY;
    FXPT2DOT30 ciexyzZ;
} CIEXYZ;
 
typedef struct {
    CIEXYZ  ciexyzRed; 
    CIEXYZ  ciexyzGreen; 
    CIEXYZ  ciexyzBlue; 
} CIEXYZTRIPLE;
 
// bitmap file header
typedef struct {
    unsigned short bfType;
    unsigned int   bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int   bfOffBits;
} BITMAPFILEHEADER;
 
// bitmap info header
typedef struct {
    unsigned int   biSize;
    unsigned int   biWidth;
    unsigned int   biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int   biCompression;
    unsigned int   biSizeImage;
    unsigned int   biXPelsPerMeter;
    unsigned int   biYPelsPerMeter;
    unsigned int   biClrUsed;
    unsigned int   biClrImportant;
    unsigned int   biRedMask;
    unsigned int   biGreenMask;
    unsigned int   biBlueMask;
    unsigned int   biAlphaMask;
    unsigned int   biCSType;
    CIEXYZTRIPLE   biEndpoints;
    unsigned int   biGammaRed;
    unsigned int   biGammaGreen;
    unsigned int   biGammaBlue;
    unsigned int   biIntent;
    unsigned int   biProfileData;
    unsigned int   biProfileSize;
    unsigned int   biReserved;
} BITMAPINFOHEADER;
 
// rgb quad
typedef struct {
    unsigned char  rgbBlue;
    unsigned char  rgbGreen;
    unsigned char  rgbRed;
    unsigned char  rgbReserved;
} RGBQUAD;

template <typename Type>
void read(std::ifstream &fp, Type &result, std::size_t size) {
    fp.read(reinterpret_cast<char*>(&result), size);
}
 
// bit extract
unsigned char bitextract(const unsigned int byte, const unsigned int mask);

class bmp_img_c {
	private:
		uint8_t *data;

		void load_dummy();
	
	public:
		int img_width, img_height;
		int img_channels;

		bmp_img_c();
		~bmp_img_c();

		bool from_file(string fname);
		void show_img_stats();
		unsigned char * get_data();
};

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

class png_img_c {
	private:
		uint8_t *data;

		void load_dummy();
	
	public:
		int img_width, img_height;
		int img_channels;

		png_img_c();
		~png_img_c();
};
