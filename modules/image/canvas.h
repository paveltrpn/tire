
#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <utility>

namespace tire {

struct Canvas {
public:
    Canvas( int32_t width, int32_t height );

    ~Canvas();

    void set_pen_size( int32_t size );
    void set_pen_color( uint8_t r, uint8_t g, uint8_t b );
    void put_pixel( int32_t x, int32_t y );
    void put_pixel_br( int32_t x, int32_t y, float br );
    void brasenham_line( std::pair<int32_t, int32_t> start,
                         std::pair<int32_t, int32_t> end );
    void wu_line( std::pair<int32_t, int32_t> start,
                  std::pair<int32_t, int32_t> end );
    void dda_line( std::pair<int32_t, int32_t> start,
                   std::pair<int32_t, int32_t> end );
    void brasenham_circle( std::pair<int32_t, int32_t> center, int32_t rd );
    int32_t get_width();
    int32_t get_height();
    int32_t get_bpp();
    uint8_t *get_data();

private:
    uint8_t *data;

    int32_t cnvs_width;
    int32_t cnvs_height;

    static constexpr uint8_t bpp = 3;

    int32_t pen_size = 1;

    uint8_t pen_color_r = 255;
    uint8_t pen_color_g = 255;
    uint8_t pen_color_b = 255;

#ifdef EXCLUDE
    int write_jpeg( std::string fname ) {
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
        FILE *outfile;           /* target file */
        JSAMPROW row_pointer[1]; /* pointer to JSAMPLE row[s] */
        int row_stride;          /* physical row width in image buffer */

        /* Step 1: allocate and initialize JPEG compression object */

        /* We have to set up the error handler first, in case the initialization
                 * step fails.  (Unlikely, but it could happen if you are out of memory.)
                 * This routine fills in the contents of struct jerr, and returns jerr's
                 * address which we place into the link field in cinfo.
                 */
        cinfo.err = jpeg_std_error( &jerr );
        /* Now we can initialize the JPEG compression object. */
        jpeg_create_compress( &cinfo );

        /* Step 2: specify data destination (eg, a file) */
        /* Note: steps 2 and 3 can be done in either order. */

        /* Here we use the library-supplied code to send compressed data to a
                 * stdio stream.  You can also write your own code to do something else.
                 * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
                 * requires it in order to write binary files.
                 */
        if ( ( outfile = fopen( fname.c_str(), "wb" ) ) == NULL ) {
            std::cout << "Canvas::write_jpeg(): can't open %s " << fname.c_str()
                      << "\n";
            return 1;
        }
        jpeg_stdio_dest( &cinfo, outfile );

        /* Step 3: set parameters for compression */

        /* First we supply a description of the input image.
                 * Four fields of the cinfo struct must be filled in:
                 */
        cinfo.image_width = cnvs_width; /* image width and height, in pixels */
        cinfo.image_height = cnvs_height;
        cinfo.input_components = bpp;   /* # of color components per pixel */
        cinfo.in_color_space = JCS_RGB; /* colorspace of input image */
        /* Now use the library's routine to set default compression parameters.
                 * (You must set at least cinfo.in_color_space before calling this,
                 * since the defaults depend on the source color space.)
                 */
        jpeg_set_defaults( &cinfo );
        /* Now you can set any non-default parameters you wish to.
                 * Here we just illustrate the use of quality (quantization table) scaling:
                 */
        // тут стоит поиграться с параметром quantization, пока что не выяснил
        // чему он должен быть равен, временно поставил - 128.
        jpeg_set_quality( &cinfo, 128,
                          TRUE /* limit to baseline-JPEG values */ );

        /* Step 4: Start compressor */

        /* TRUE ensures that we will write a complete interchange-JPEG file.
                 * Pass TRUE unless you are very sure of what you're doing.
                 */
        jpeg_start_compress( &cinfo, TRUE );

        /* Step 5: while (scan lines remain to be written) */
        /*           jpeg_write_scanlines(...); */

        /* Here we use the library's state variable cinfo.next_scanline as the
                 * loop counter, so that we don't have to keep track ourselves.
                 * To keep things simple, we pass one scanline per call; you can pass
                 * more if you wish, though.
                 */
        row_stride = cnvs_width * bpp; /* JSAMPLEs per row in image_buffer */

        while ( cinfo.next_scanline < cinfo.image_height ) {
            /* jpeg_write_scanlines expects an array of pointers to scanlines.
                     * Here the array is only one element long, but you could pass
                     * more than one scanline at a time if that's more convenient.
                     */
            row_pointer[0] = &data[cinfo.next_scanline * row_stride];
            (void)jpeg_write_scanlines( &cinfo, row_pointer, 1 );
        }

        /* Step 6: Finish compression */

        jpeg_finish_compress( &cinfo );
        /* After finish_compress, we can close the output file. */
        fclose( outfile );

        /* Step 7: release JPEG compression object */

        /* This is an important step since it will release a good deal of memory. */
        jpeg_destroy_compress( &cinfo );

        /* And we're done! */

        return 0;
    }

    int write_tga( std::string fname ) {
        TGA *tga_out;
        TGAData tga_data;

        std::memset( &tga_data, 0, sizeof( TGAData ) );

        // Подготовливаем файл для записи библиотечной функцией.
        // Она открывает файл на диске, и хранит его дискриптор в структуре TGA
        tga_out = TGAOpen( fname.c_str(), "wb" );

        // Сообщаем параметры записываемого массива
        tga_out->hdr.depth = bpp * 8;
        tga_out->hdr.width = cnvs_width;
        tga_out->hdr.height = cnvs_height;

        // В этом поле не должно быть 0, или TGAWriteScanlines() вернёт TGA_OK
        // и не будет ничего писать в файл кроме заголовка
        // значения этого поля из документации на libtga такие:
        // 0 = no image data included
        // 1 = color mapped
        // 2 = grayscale
        // 3 = true-color (RGB or RGBA)
        // 9 = RLE encoded color mapped
        // 10 = RLE encoded grayscale
        // 11 = RLE encoded true-color
        // но нормально записывается только при tga_out->hdr.img_t = 10,
        // при других значениях файл не читался. Это поле, по видимому, зависит
        // от наличия флага TGA_RLE_ENCODE в структуре TGAData
        tga_out->hdr.img_t = 10;

        tga_data.img_data = data;

        // Устанавливаем опции с которыми будет производиться запись массива на диск.
        tga_data.flags =
            TGA_IMAGE_DATA | TGA_RLE_ENCODE | TGA_IMAGE_ID | TGA_RGB;

        // Записываем массив на диск с установленными опциями.
        // TGAWriteImage() вызывает все необходимые действия - пишет заголовок и т.д.
        if ( TGAWriteImage( tga_out, &tga_data ) != TGA_OK ) {
            std::cout << "Canvas::write_jpeg(): TGAWriteImage() return error"
                      << "\n";
        }

        // Записываем только заголовок и массив рисунка.
        // TGAWriteImage() кроме этого вызывает ещё несколько функций.
        //
        // TGAWriteHeader(tga_out);
        //
        // if (TGAWriteScanlines(tga_out, &tga_data) != TGA_OK) {
        // std::cout << "Canvas::write_jpeg(): TGAWriteScanlines() return error" << "\n";
        // }

        TGAClose( tga_out );

        return 0;
    }
#endif
};
}  // namespace tire
