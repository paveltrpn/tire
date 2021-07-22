
#ifndef __load_jpeg_h__
#define __load_jpeg_h__

#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>

#include <jpeg/jpeglib.h>    
#include <jpeg/jerror.h>
#include <fmt/format.h>

#include "base64.h"

class CJpegBitmap {
	private:
		uint8_t* decompressed;
	
		uint32_t bitmapWidth;
		uint32_t bitmapHeight;
		uint32_t bitmapDepth;
		
	public:
		CJpegBitmap() {
            decompressed = nullptr;
        };

		~CJpegBitmap() {
            if (decompressed != nullptr) {
		        delete[] decompressed;
            }
        };

		bool readFromFile(const std::string& fname);
        bool readFromBase64(const::std::string& base64String);
		bool writeToFile(const std::string& fname);

		size_t      getWidht();
        size_t      getHeight(); 
        size_t      getChanelsCount();
        uint8_t*    getDataPtr();
};

bool CJpegBitmap::readFromFile(const std::string& fname) {
	struct jpeg_decompress_struct info;
	struct jpeg_error_mgr err;				//the error handler
	unsigned char * rowptr[1];				//pointer to an array
	FILE *fin;
	
	fin = fopen(fname.c_str(), "rb");
	if (!fin) {
		std::cout << "CJpegBitmap::fromFile(): ERROR! Unable to open file - " << fname << std::endl;
		return false;
	}
	
	info.err = jpeg_std_error(&err);
	jpeg_create_decompress(&info); 
	
	jpeg_stdio_src(&info, fin);
	jpeg_read_header(&info, TRUE);
	
	jpeg_start_decompress(&info);

	bitmapWidth = info.output_width;
	bitmapHeight = info.output_height;
	bitmapDepth = info.num_components;
	decompressed = new uint8_t [info.output_width * info.output_height * info.num_components];

	while (info.output_scanline < info.output_height) {
		rowptr[0] = (unsigned char *)decompressed + info.num_components * info.output_width * info.output_scanline; 
		jpeg_read_scanlines(&info, rowptr, 1);
	}
	
	jpeg_finish_decompress(&info);
	
	jpeg_destroy_decompress(&info);
	
	fclose(fin);
	
	return true;
}

bool CJpegBitmap::readFromBase64(const std::string& base64String) {
    struct jpeg_decompress_struct info;
	struct jpeg_error_mgr err;				//the error handler
	unsigned char * rowptr[1];				//pointer to an array
    // Декодирование строки из base64 кода, т.е. `base64String` - это "бинарный файл" на диске, а
    // `decodedStr` - "считанный файл".
    std::string decodedStr = base64_decode(base64String);
		
	info.err = jpeg_std_error(&err);
	jpeg_create_decompress(&info); 
	
    // Тут происходит дич в виде каста `const char *` в `unsigned char *` и
    // чтение jpeg файла из буфера в памяти. Этот буфер представляет из себя строку,
    // полученую из другой строки, закодированной как base64. 
    jpeg_mem_src(&info, reinterpret_cast<unsigned char*>(const_cast<char*>(decodedStr.c_str())), decodedStr.length());
	jpeg_read_header(&info, TRUE);
	
	jpeg_start_decompress(&info);

	bitmapWidth = info.output_width;
	bitmapHeight = info.output_height;
	bitmapDepth = info.num_components;
	decompressed = new uint8_t [info.output_width * info.output_height * info.num_components];

	while (info.output_scanline < info.output_height) {
		rowptr[0] = (unsigned char *)decompressed + info.num_components * info.output_width * info.output_scanline; 
		jpeg_read_scanlines(&info, rowptr, 1);
	}
	
	jpeg_finish_decompress(&info);
	
	jpeg_destroy_decompress(&info);
	
	return true;
}

bool CJpegBitmap::writeToFile(const std::string& fname) {
    /*
    * Sample routine for JPEG compression.  We assume that the target file name
    * and a compression quality factor are passed in.
    */

    /* This struct contains the JPEG compression parameters and pointers to
    * working space (which is allocated as needed by the JPEG library).
    * It is possible to have several such structures, representing multiple
    * compression/decompression processes, in existence at once.  We refer
    * to any one struct (and its associated working data) as a "JPEG object".
    */
    struct jpeg_compress_struct cinfo;
    /* This struct represents a JPEG error handler.  It is declared separately
    * because applications often want to supply a specialized error handler
    * (see the second half of this file for an example).  But here we just
    * take the easy way out and use the standard error handler, which will
    * print a message on stderr and call exit() if compression fails.
    * Note that this struct must live as long as the main JPEG parameter
    * struct, to avoid dangling-pointer problems.
    */
    struct jpeg_error_mgr jerr;
    /* More stuff */
    FILE * outfile;		/* target file */
    JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
    int row_stride;		/* physical row width in image buffer */

    /* Step 1: allocate and initialize JPEG compression object */

    /* We have to set up the error handler first, in case the initialization
    * step fails.  (Unlikely, but it could happen if you are out of memory.)
    * This routine fills in the contents of struct jerr, and returns jerr's
    * address which we place into the link field in cinfo.
    */
    cinfo.err = jpeg_std_error(&jerr);
    /* Now we can initialize the JPEG compression object. */
    jpeg_create_compress(&cinfo);

    /* Step 2: specify data destination (eg, a file) */
    /* Note: steps 2 and 3 can be done in either order. */

    /* Here we use the library-supplied code to send compressed data to a
    * stdio stream.  You can also write your own code to do something else.
    * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
    * requires it in order to write binary files.
    */

   	if (decompressed == nullptr) {
		std::cout << fmt::format("CJpegBitmap::writeToFile(): Error! data to writing is empty!\n");
		return false;
	}

    if ((outfile = fopen(fname.c_str(), "wb")) == NULL) {
        std::cout << "canvas_c::write_jpeg(): can't open %s " << fname.c_str() << "\n";
        return false;
    }
    jpeg_stdio_dest(&cinfo, outfile);

    /* Step 3: set parameters for compression */

    /* First we supply a description of the input image.
    * Four fields of the cinfo struct must be filled in:
    */
    cinfo.image_width = bitmapWidth; 	/* image width and height, in pixels */
    cinfo.image_height = bitmapHeight;
    cinfo.input_components = bitmapDepth;		/* # of color components per pixel */
    cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
    /* Now use the library's routine to set default compression parameters.
    * (You must set at least cinfo.in_color_space before calling this,
    * since the defaults depend on the source color space.)
    */
    jpeg_set_defaults(&cinfo);
    /* Now you can set any non-default parameters you wish to.
    * Here we just illustrate the use of quality (quantization table) scaling:
    */
    // тут стоит поиграться с параметром quantization, пока что не выяснил 
    // чему он должен быть равен, временно поставил - 128.
    jpeg_set_quality(&cinfo, 128, TRUE /* limit to baseline-JPEG values */);

    /* Step 4: Start compressor */

    /* TRUE ensures that we will write a complete interchange-JPEG file.
    * Pass TRUE unless you are very sure of what you're doing.
    */
    jpeg_start_compress(&cinfo, TRUE);

    /* Step 5: while (scan lines remain to be written) */
    /*           jpeg_write_scanlines(...); */

    /* Here we use the library's state variable cinfo.next_scanline as the
    * loop counter, so that we don't have to keep track ourselves.
    * To keep things simple, we pass one scanline per call; you can pass
    * more if you wish, though.
    */
    row_stride = bitmapWidth * bitmapDepth;	/* JSAMPLEs per row in image_buffer */

    while (cinfo.next_scanline < cinfo.image_height) {
        /* jpeg_write_scanlines expects an array of pointers to scanlines.
        * Here the array is only one element long, but you could pass
        * more than one scanline at a time if that's more convenient.
        */
        row_pointer[0] = &decompressed[cinfo.next_scanline * row_stride];
        (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    /* Step 6: Finish compression */

    jpeg_finish_compress(&cinfo);
    /* After finish_compress, we can close the output file. */
    fclose(outfile);

    /* Step 7: release JPEG compression object */

    /* This is an important step since it will release a good deal of memory. */
    jpeg_destroy_compress(&cinfo);

    /* And we're done! */

    return true;
}

size_t CJpegBitmap::getWidht() {
    return bitmapWidth;
}

size_t CJpegBitmap::getHeight() {
    return bitmapHeight;
}

size_t CJpegBitmap::getChanelsCount() {
    return bitmapDepth;
}

uint8_t * CJpegBitmap::getDataPtr() {
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

#endif