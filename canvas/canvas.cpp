
#include "canvas.h"
#include <stdio.h>
#include "jpeglib.h"

void canvas_c::set_pen_size(int32_t size) {
    pen_size = size;
}

void canvas_c::set_pen_color(uint8_t r, uint8_t g, uint8_t b) {
    pen_color_r = r;
    pen_color_g = g;
    pen_color_b = b;
}

void canvas_c::put_pixel(int32_t x, int32_t y) {
    if ((x < 0) || (y < 0) || (x >= cnvs_width) || (y >= cnvs_width)) return;

    data[((x*bpp)*cnvs_height + y*bpp) + 0] = pen_color_r;
    data[((x*bpp)*cnvs_height + y*bpp) + 1] = pen_color_g;
    data[((x*bpp)*cnvs_height + y*bpp) + 2] = pen_color_b;
}

void canvas_c::put_pixel_depth(int32_t x, int32_t y, float intence) {
    if ((x < 0) || (y < 0) || (x >= cnvs_width) || (y >= cnvs_width)) return;

    data[((x*bpp)*cnvs_height + y*bpp) + 0] = uint8_t(std::ceil(pen_color_r * intence));
    data[((x*bpp)*cnvs_height + y*bpp) + 1] = uint8_t(std::ceil(pen_color_g * intence));
    data[((x*bpp)*cnvs_height + y*bpp) + 2] = uint8_t(std::ceil(pen_color_b * intence));
}

void canvas_c::brasenham_line(std::pair<int32_t, int32_t> start, std::pair<int32_t, int32_t> end) {
    int32_t dX = abs(end.first - start.first);
    int32_t dY = abs(end.second - start.second);
    int32_t signX;
    int32_t signY;
    int32_t err2, err = dX - dY;
    std::pair<int32_t, int32_t> now_point = start;

    if (start.first < end.first) {
        signX = 1;
    } else {
        signX = -1;
    }

    if (start.second < end.second) {
        signY = 1;
    } else {
        signY = -1;
    }

    put_pixel(end.first, end.second);

    while ((now_point.first != end.first) || (now_point.second != end.second)) {
        put_pixel(now_point.first, now_point.second);

        err2 = err * 2;

        if(err2 > -dY) {
            err -= dY;
            now_point.first += signX;
        }

        if(err2 < dX) {
            err += dX;
            now_point.second += signY;
        }
    }
}

int32_t ipart(float a) {
    float rt;

    std::modf(a, &rt);

    return int32_t(rt);
}

float fpart(float a) {
    float tmp;
    return std::modf(a, &tmp);
}

void canvas_c::wu_line(std::pair<int32_t, int32_t> start, std::pair<int32_t, int32_t> end) {
    if (end.first < start.first) {
        std::swap(start, end);
    }

    int32_t dx = end.first - start.first;
    int32_t dy = end.second - start.second;
    float gradient = dx/dy;

    int32_t xend = std::round(start.first);
    int32_t yend = start.second + gradient * (xend - start.first);
    float xgap = 1 - fpart(start.first + 0.5f);
    int32_t xpxl1 = xend;  // будет использоваться в основном цикле
    int32_t ypxl1 = ipart(yend);
    put_pixel_depth(xpxl1, ypxl1, (1 - fpart(yend)) * xgap);
    put_pixel_depth(xpxl1, ypxl1 + 1, fpart(yend) * xgap);
    int32_t intery = yend + gradient; // первое y-пересечение для цикла

    xend = std::round(end.first);
    yend = end.second + gradient * (xend - end.first);
    xgap = fpart(end.first + 0.5);
    int32_t xpxl2 = xend;  // будет использоваться в основном цикле
    int32_t ypxl2 = ipart(yend);
    put_pixel_depth(xpxl2, ypxl2, (1 - fpart(yend)) * xgap);
    put_pixel_depth(xpxl2, ypxl2 + 1, fpart(yend) * xgap);

    for (int32_t i = xpxl1 + 1; i < xpxl2 - 1; i++) {
        put_pixel_depth(i, ipart(intery), 1 - fpart(intery));
        put_pixel_depth(i, ipart(intery) + 1, fpart(intery));
        intery = intery + gradient;
    }
}

void canvas_c::brasenham_circle(std::pair<int32_t, int32_t> center, int32_t rd) {
    int32_t x = 0;
    int32_t y = rd;
    int32_t delta = 1 -2 * rd;
    int error = 0;

    while (y >= 0) {
        put_pixel(center.first + x, center.second + y);
        put_pixel(center.first + x, center.second - y);
        put_pixel(center.first - x, center.second + y);
        put_pixel(center.first - x, center.second - y);

        error =2 * (delta + y) - 1;

        if ((delta < 0) && (error <= 0)) {
            delta += 2 * ++x + 1;
            continue;
        }

        if ((delta > 0) && (error > 0)) {
            delta -= 2 * --y + 1;
            continue;
        }

        delta += 2 * (++x - --y);
    }
}

void canvas_c::rect(std::pair<int32_t, int32_t> ul,
                    std::pair<int32_t, int32_t> ur,
                    std::pair<int32_t, int32_t> dl,
                    std::pair<int32_t, int32_t> dr) {

}
        
int canvas_c::write_jpeg(std::string fname) {
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
    if ((outfile = fopen(fname.c_str(), "wb")) == NULL) {
        fprintf(stderr, "can't open %s\n", fname.c_str());
        return 1;
    }
    jpeg_stdio_dest(&cinfo, outfile);

    /* Step 3: set parameters for compression */

    /* First we supply a description of the input image.
    * Four fields of the cinfo struct must be filled in:
    */
    cinfo.image_width = cnvs_width; 	/* image width and height, in pixels */
    cinfo.image_height = cnvs_height;
    cinfo.input_components = bpp;		/* # of color components per pixel */
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
    row_stride = cnvs_width * bpp;	/* JSAMPLEs per row in image_buffer */

    while (cinfo.next_scanline < cinfo.image_height) {
        /* jpeg_write_scanlines expects an array of pointers to scanlines.
        * Here the array is only one element long, but you could pass
        * more than one scanline at a time if that's more convenient.
        */
        row_pointer[0] = & data[cinfo.next_scanline * row_stride];
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

    return 0;
}
