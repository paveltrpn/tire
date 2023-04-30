
#include <filesystem>

#include "bitmap.h"

namespace fs = std::filesystem;

unsigned char bitextract(const unsigned int byte, const unsigned int mask) {
    if (mask == 0) {
        return 0;
    }

    // определение количества нулевых бит справа от маски
    unsigned int maskBufer = mask, maskPadding = 0;

    while (!(maskBufer & 1)) {
        maskBufer >>= 1;
        maskPadding++;
    }

    // применение маски и смещение
    return (byte & mask) >> maskPadding;
}

bool CBitmap::readFromTgaFile(const std::string& fname) {
    TGA* in = TGAOpen(fname.c_str(), "rb");
    TGAData in_data;

    std::memset(&in_data, 0, sizeof(TGAData));
    in_data.flags = TGA_IMAGE_ID | TGA_IMAGE_DATA | TGA_RGB;

    TGAReadImage(in, &in_data);

    if (!TGA_SUCCEEDED(in)) {
        std::cout << "CBitmap::readFromTgaFile(): ERROR! Unable to open file - " << fname
                  << std::endl;
        return false;
    }

    std::cout << "CBitmap::readFromTgaFile(): load bitmap from file - " << fname << std::endl;

    bitmapName = fname;
    bitmapName = fname;
    bitmapWidth = in->hdr.width;
    bitmapHeight = in->hdr.height;
    bitmapChannels = in->hdr.depth / 8;

    decompressed = new uint8_t[bitmapWidth * bitmapHeight * (bitmapChannels)];

    std::memcpy(decompressed,
                static_cast<uint8_t*>(in_data.img_data),
                bitmapWidth * bitmapHeight * (bitmapChannels));

    TGAFreeTGAData(&in_data);
    TGAClose(in);

    return true;
}

bool CBitmap::readFromJpegFile(const std::string& fname) {
    struct jpeg_decompress_struct info;
    struct jpeg_error_mgr err;  // the error handler
    unsigned char* rowptr[1];   // pointer to an array
    FILE* fin;

    fin = fopen(fname.c_str(), "rb");
    if (!fin) {
        std::cout << "CBitmap::readFromJpegFile(): ERROR! Unable to open file - " << fname
                  << std::endl;
        return false;
    }

    std::cout << "CBitmap::readFromJpegFile(): load bitmap from file - " << fname << std::endl;

    bitmapName = fname;

    info.err = jpeg_std_error(&err);
    jpeg_create_decompress(&info);

    jpeg_stdio_src(&info, fin);
    jpeg_read_header(&info, TRUE);

    jpeg_start_decompress(&info);

    bitmapWidth = info.output_width;
    bitmapHeight = info.output_height;
    bitmapChannels = info.num_components;
    decompressed = new uint8_t[info.output_width * info.output_height * info.num_components];

    while (info.output_scanline < info.output_height) {
        rowptr[0] = (unsigned char*)decompressed
                    + info.num_components * info.output_width * info.output_scanline;
        jpeg_read_scanlines(&info, rowptr, 1);
    }

    jpeg_finish_decompress(&info);

    jpeg_destroy_decompress(&info);

    fclose(fin);

    return true;
}

bool CBitmap::readFromBmpFile(const std::string& fname) {
    std::ifstream fileStream(fname, std::ifstream::binary);
    BITMAPFILEHEADER file_header;
    BITMAPINFOHEADER info_header;

    if (!fileStream) {
        std::cout << "CBitmap::readFromBmpFile(): Error opening file '" << fname << "."
                  << std::endl;
        return 0;
    }

    // заголовк изображения
    read(fileStream, file_header.bfType, sizeof(file_header.bfType));
    read(fileStream, file_header.bfSize, sizeof(file_header.bfSize));
    read(fileStream, file_header.bfReserved1, sizeof(file_header.bfReserved1));
    read(fileStream, file_header.bfReserved2, sizeof(file_header.bfReserved2));
    read(fileStream, file_header.bfOffBits, sizeof(file_header.bfOffBits));

    if (file_header.bfType != 0x4D42) {
        std::cout << "CBitmap::readFromBmpFile(): Error -'" << fname << "' is not BMP file."
                  << std::endl;
        return 0;
    }

    // информация изображения
    read(fileStream, info_header.biSize, sizeof(info_header.biSize));

    // bmp core
    if (info_header.biSize >= 12) {
        read(fileStream, info_header.biWidth, sizeof(info_header.biWidth));
        read(fileStream, info_header.biHeight, sizeof(info_header.biHeight));
        read(fileStream, info_header.biPlanes, sizeof(info_header.biPlanes));
        read(fileStream, info_header.biBitCount, sizeof(info_header.biBitCount));
    }

    // получаем информацию о битности
    int colorsCount = info_header.biBitCount >> 3;
    if (colorsCount < 3) {
        colorsCount = 3;
    }

    int bitsOnColor = info_header.biBitCount / colorsCount;
    int maskValue = (1 << bitsOnColor) - 1;

    // bmp v1
    if (info_header.biSize >= 40) {
        read(fileStream, info_header.biCompression, sizeof(info_header.biCompression));
        read(fileStream, info_header.biSizeImage, sizeof(info_header.biSizeImage));
        read(fileStream, info_header.biXPelsPerMeter, sizeof(info_header.biXPelsPerMeter));
        read(fileStream, info_header.biYPelsPerMeter, sizeof(info_header.biYPelsPerMeter));
        read(fileStream, info_header.biClrUsed, sizeof(info_header.biClrUsed));
        read(fileStream, info_header.biClrImportant, sizeof(info_header.biClrImportant));
    }

    // bmp v2
    info_header.biRedMask = 0;
    info_header.biGreenMask = 0;
    info_header.biBlueMask = 0;

    if (info_header.biSize >= 52) {
        read(fileStream, info_header.biRedMask, sizeof(info_header.biRedMask));
        read(fileStream, info_header.biGreenMask, sizeof(info_header.biGreenMask));
        read(fileStream, info_header.biBlueMask, sizeof(info_header.biBlueMask));
    }

    // если маска не задана, то ставим маску по умолчанию
    if (info_header.biRedMask == 0 || info_header.biGreenMask == 0 || info_header.biBlueMask == 0) {
        info_header.biRedMask = maskValue << (bitsOnColor * 2);
        info_header.biGreenMask = maskValue << bitsOnColor;
        info_header.biBlueMask = maskValue;
    }

    // bmp v3
    if (info_header.biSize >= 56) {
        read(fileStream, info_header.biAlphaMask, sizeof(info_header.biAlphaMask));
    } else {
        info_header.biAlphaMask = maskValue << (bitsOnColor * 3);
    }

    // bmp v4
    if (info_header.biSize >= 108) {
        read(fileStream, info_header.biCSType, sizeof(info_header.biCSType));
        read(fileStream, info_header.biEndpoints, sizeof(info_header.biEndpoints));
        read(fileStream, info_header.biGammaRed, sizeof(info_header.biGammaRed));
        read(fileStream, info_header.biGammaGreen, sizeof(info_header.biGammaGreen));
        read(fileStream, info_header.biGammaBlue, sizeof(info_header.biGammaBlue));
    }

    // bmp v5
    if (info_header.biSize >= 124) {
        read(fileStream, info_header.biIntent, sizeof(info_header.biIntent));
        read(fileStream, info_header.biProfileData, sizeof(info_header.biProfileData));
        read(fileStream, info_header.biProfileSize, sizeof(info_header.biProfileSize));
        read(fileStream, info_header.biReserved, sizeof(info_header.biReserved));
    }

    // проверка на поддерку этой версии формата
    if (info_header.biSize != 12 && info_header.biSize != 40 && info_header.biSize != 52
        && info_header.biSize != 56 && info_header.biSize != 108 && info_header.biSize != 124) {
        std::cout << "CBitmap::readFromBmpFile(): Error - Unsupported BMP format." << std::endl;
        return 0;
    }

    if (info_header.biBitCount != 16 && info_header.biBitCount != 24
        && info_header.biBitCount != 32) {
        std::cout << "CBitmap::readFromBmpFile(): Error - Unsupported BMP bit count." << std::endl;
        return 0;
    }

    if (info_header.biCompression != 0 && info_header.biCompression != 3) {
        std::cout << "CBitmap::readFromBmpFile(): Error - Unsupported BMP compression."
                  << std::endl;
        return 0;
    }

    // определение размера отступа в конце каждой строки
    int linePadding = ((info_header.biWidth * (info_header.biBitCount / 8)) % 4) & 3;

    uint32_t bufer;
    size_t i, j;

    /*  Чтение файла в массив uint8_t *decompressed */
    decompressed = new uint8_t[info_header.biHeight * info_header.biWidth * 4];

    for (i = 0; i < info_header.biHeight; i++) {
        for (j = 0; j < info_header.biWidth; j++) {
            read(fileStream, bufer, info_header.biBitCount / 8);

            decompressed[((i * 4) * info_header.biHeight + j * 4) + 2]
              = bitextract(bufer, info_header.biRedMask);
            decompressed[((i * 4) * info_header.biHeight + j * 4) + 1]
              = bitextract(bufer, info_header.biGreenMask);
            decompressed[((i * 4) * info_header.biHeight + j * 4) + 0]
              = bitextract(bufer, info_header.biBlueMask);
            decompressed[((i * 4) * info_header.biHeight + j * 4) + 3]
              = bitextract(bufer, info_header.biAlphaMask);
        }
        fileStream.seekg(linePadding, std::ios_base::cur);
    }

    bitmapWidth = info_header.biWidth;
    bitmapHeight = info_header.biHeight;
    bitmapChannels = info_header.biBitCount / 8;
    bitmapName = fname;

    fileStream.close();

    return true;
}

bool CBitmap::readFromFile(const std::string& fname) {
    if (!fs::exists(fname)) {
        std::cout << "CBitmap::readFromFile(): ERROR! file - " << fname << " does not exist!"
                  << std::endl;
        loadDummyCheckerPattern();
        std::cout << "CBitmap::readFromFile(): dummy image is loaded instead of - " << fname
                  << std::endl;
        return false;
    }

    auto tgaLiteralPos = fname.find(".tga");
    auto jpegLiteralPos = fname.find(".jpg");
    auto bmpLiteralPos = fname.find(".bmp");

    if (jpegLiteralPos != std::string::npos) {
        readFromJpegFile(fname);
        return true;
    };

    if (tgaLiteralPos != std::string::npos) {
        readFromTgaFile(fname);
        return true;
    }

    if (bmpLiteralPos != std::string::npos) {
        readFromBmpFile(fname);
        return true;
    }

    std::cout << "CBitmap::readFromFile(): ERROR! unknown file format - " << fname << std::endl;
    loadDummyCheckerPattern();
    std::cout << "CBitmap::readFromFile(): dummy image is loaded instead of - " << fname
              << std::endl;

    return false;
}

bool CBitmap::readFromJpegBase64(const std::string& base64String) {
    struct jpeg_decompress_struct info;
    struct jpeg_error_mgr err;  // the error handler
    unsigned char* rowptr[1];   // pointer to an array
    // Декодирование строки из base64 кода, т.е. `base64String` - это "бинарный файл" на диске, а
    // `decodedStr` - "считанный файл".
    std::string decodedStr = base64_decode(base64String);

    info.err = jpeg_std_error(&err);
    jpeg_create_decompress(&info);

    // Тут происходит каст `const char *` в `unsigned char *` (что есть UB, лучше копировать) и
    // чтение jpeg файла из буфера в памяти. Этот буфер представляет из себя строку,
    // полученую из другой строки, закодированной как base64.
    jpeg_mem_src(&info,
                 reinterpret_cast<unsigned char*>(const_cast<char*>(decodedStr.c_str())),
                 decodedStr.length());
    jpeg_read_header(&info, TRUE);

    jpeg_start_decompress(&info);

    bitmapWidth = info.output_width;
    bitmapHeight = info.output_height;
    bitmapChannels = info.num_components;
    decompressed = new uint8_t[info.output_width * info.output_height * info.num_components];

    while (info.output_scanline < info.output_height) {
        rowptr[0] = (unsigned char*)decompressed
                    + info.num_components * info.output_width * info.output_scanline;
        jpeg_read_scanlines(&info, rowptr, 1);
    }

    jpeg_finish_decompress(&info);

    jpeg_destroy_decompress(&info);

    return true;
}

bool CBitmap::writeToJpegFile(const std::string& fname) {
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
    FILE* outfile;           /* target file */
    JSAMPROW row_pointer[1]; /* pointer to JSAMPLE row[s] */
    int row_stride;          /* physical row width in image buffer */

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
        std::cout << fmt::format("CBitmap::writeToFile(): Error! data to writing is empty!\n");
        return false;
    }

    if ((outfile = fopen(fname.c_str(), "wb")) == NULL) {
        std::cout << "CBitmap::writeToFile: can't open %s " << fname.c_str() << "\n";
        return false;
    }
    jpeg_stdio_dest(&cinfo, outfile);

    /* Step 3: set parameters for compression */

    /* First we supply a description of the input image.
     * Four fields of the cinfo struct must be filled in:
     */
    cinfo.image_width = bitmapWidth; /* image width and height, in pixels */
    cinfo.image_height = bitmapHeight;
    cinfo.input_components = bitmapChannels; /* # of color components per pixel */
    cinfo.in_color_space = JCS_RGB;          /* colorspace of input image */
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
    row_stride = bitmapWidth * bitmapChannels; /* JSAMPLEs per row in image_buffer */

    while (cinfo.next_scanline < cinfo.image_height) {
        /* jpeg_write_scanlines expects an array of pointers to scanlines.
         * Here the array is only one element long, but you could pass
         * more than one scanline at a time if that's more convenient.
         */
        row_pointer[0] = &decompressed[cinfo.next_scanline * row_stride];
        (void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
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

size_t CBitmap::getWidht() {
    return bitmapWidth;
}

size_t CBitmap::getHeight() {
    return bitmapHeight;
}

size_t CBitmap::getChanelsCount() {
    return bitmapChannels;
}

uint8_t* CBitmap::getDataPtr() {
    if (decompressed != nullptr) {
        return decompressed;
    } else {
        std::cout << "CBitmap::getDataPtr(): File is empty!" << std::endl;
        return nullptr;
    }
}

void CBitmap::loadDummyCheckerPattern() {
    constexpr uint32_t dummySize = 512;
    constexpr uint32_t dummyChannels = 3;
    constexpr int32_t checkerSquareSizePx = 8;

    bitmapWidth = bitmapHeight = dummySize;
    bitmapChannels = dummyChannels;

    int32_t value;

    decompressed = new uint8_t[bitmapWidth * bitmapHeight * bitmapChannels];

    for (size_t i = 0; i < bitmapHeight; i++)
        for (size_t j = 0; j < bitmapWidth; j++) {
            value = (((i & checkerSquareSizePx) == 0) ^ ((j & checkerSquareSizePx) == 0)) * 255;
            decompressed[((i * bitmapChannels) * bitmapHeight + j * bitmapChannels) + 0] = value;
            decompressed[((i * bitmapChannels) * bitmapHeight + j * bitmapChannels) + 1] = value;
            decompressed[((i * bitmapChannels) * bitmapHeight + j * bitmapChannels) + 2] = value;
            if (bitmapChannels == 4) {
                decompressed[((i * bitmapChannels) * bitmapHeight + j * bitmapChannels) + 3]
                  = value;
            }
        }
}