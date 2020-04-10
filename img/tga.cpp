
#include "tga.h"

static const uint32_t TGA_IMAGE_TYPE_NO_IMAGE=0;
static const uint32_t TGA_IMAGE_TYPE_PALETTE=1;
static const uint32_t TGA_IMAGE_TYPE_RGB=2;
static const uint32_t TGA_IMAGE_TYPE_GRAYSCALE=3;
static const uint32_t TGA_IMAGE_TYPE_MASK=(1<<2)|(1<<1)|(1<<0);
static const uint32_t TGA_IMAGE_TYPE_RLE_MASK=(1<<3);

static const uint32_t TGA_DESCRIPTOR_FORWARD=32;
static const uint32_t TGA_DESCRIPTOR_REVERSE=8;

static const uint32_t TGA_COLOR_MAP_NO_PALETTE=0;
static const uint32_t TGA_COLOR_MAP_PALETTE=1;

tga_img_c::tga_img_c() {
	img_width = img_height = img_channels = 0;
	data = nullptr;
}

tga_img_c::~tga_img_c() {
	if (data != nullptr) {
		delete[] data;
	}
}

bool tga_img_c::from_file(string fname) {
    STGAHeader sTGAHeader;
    ifstream fileStream;
    unsigned char BlockInfo, isPacked, R, G, B, A;
    unsigned int NumPixels;
    size_t i, k;

    fileStream.open(fname, std::ifstream::binary);
    if (!fileStream) {
        std::cout << "tga_img_c::from_file(): Error opening file '" << fname << "." << std::endl;
        return false;
    }

    fileStream.read(reinterpret_cast<char*>(&sTGAHeader), sizeof(STGAHeader));

    img_width = sTGAHeader.width;
    img_height = sTGAHeader.height;
    img_channels = sTGAHeader.bits/8;

    data = new uint8_t[img_width*img_height*img_channels];

    // Non RLE
    if (sTGAHeader.imageType == 2) {
        for (i = 0; i < img_width * img_height; i++)
        {
            fileStream.read(reinterpret_cast<char*>(&R), 1);
            fileStream.read(reinterpret_cast<char*>(&G), 1);
            fileStream.read(reinterpret_cast<char*>(&B), 1);
            
            if (img_channels == 4) {
                fileStream.read(reinterpret_cast<char*>(&A), 1);
            }

            data[i * img_channels + 0] = B;
            data[i * img_channels + 1] = G;
            data[i * img_channels + 2] = R;
            if (img_channels == 4) {
                data[i * img_channels + 3] = A;
            }
        }
    } 

    //RLE
    if (sTGAHeader.imageType == 10) { 
        for (i = 0; i < img_width * img_height;) {
            fileStream.read(reinterpret_cast<char*>(&BlockInfo), 1);

            isPacked = BlockInfo & 128;
            NumPixels = BlockInfo & 127;

            //Если запакованные данные
            if (isPacked) {
                fileStream.read(reinterpret_cast<char*>(&R), 1);
                fileStream.read(reinterpret_cast<char*>(&G), 1);
                fileStream.read(reinterpret_cast<char*>(&B), 1);
                if(img_channels == 4) {
                    fileStream.read(reinterpret_cast<char*>(&A), 1);
                }

                for(k = 0; k < NumPixels + 1; k++) {
                    data[i * img_channels + 0] = B;
                    data[i * img_channels + 1] = G;
                    data[i * img_channels + 2] = R;

                    if(img_channels == 4) {
                        data[i * img_channels + 3] = A;
                    }
                    i++;
                }

            } else { //Если незапакованные
                for (k = 0; k < NumPixels + 1; k++) {
                    fileStream.read(reinterpret_cast<char*>(&R), 1);
                    fileStream.read(reinterpret_cast<char*>(&G), 1);
                    fileStream.read(reinterpret_cast<char*>(&B), 1);
                    if (img_channels == 4) {
                        fileStream.read(reinterpret_cast<char*>(&A), 1);
                    }
                    data[i * img_channels + 0] = B;
                    data[i * img_channels + 1] = G;
                    data[i * img_channels + 2] = R;
                    if (img_channels == 4) {
                        data[i * img_channels + 3] = A;
                    }
                    i++;
                }
            }
        }
    }

    fileStream.close();

    return false;
}

void tga_img_c::show_img_stats() {
    if (data != nullptr) {
		std::cout << "TGA stats for file" << std::endl; 
		std::cout << "width - " << img_width << "\n" <<
					"height - " << img_height << "\n" <<
					"channels - " << img_channels << std::endl;
	} else std::cout << "File is empty!" << std::endl;
}

/*
    Взято из репозитория https://github.com/da-nie/SoftwareGraphicsLibrary/
	Статья на хабре - https://habr.com/ru/post/495664/

bool tga_img_c::__from_file(string fname) {
    std::ifstream fileStream(fname, std::ifstream::binary);
    if (!fileStream) {
        std::cout << "tga_img_c::from_file(): Error opening file '" << fname << "." << std::endl;
        return 0;
    }

    // определяем размер файла 
    fileStream.seekg(0,std::ios_base::end);
    uint32_t file_size = static_cast<uint32_t>(fileStream.tellg());
    fileStream.seekg(0,std::ios_base::beg);

    uint8_t *data_ptr = new uint8_t[file_size+1];

    //  читаем весь файл разом в временною переменную uint8_t *data_ptr 
    fileStream.read(reinterpret_cast<char*>(data_ptr), sizeof(uint8_t)*file_size);

    fileStream.close();

    STGAHeader sTGAHeader;
 
    // если размер считанного файла меньше
    // размера заголовка tga 
    if (file_size < sizeof(STGAHeader)) {
        return false;
    }

    memcpy(&sTGAHeader,data_ptr,sizeof(STGAHeader));
    
    if (sTGAHeader.imageType&TGA_IMAGE_TYPE_RLE_MASK) {
        return false;//RLE
    }

    if ((sTGAHeader.imageType&TGA_IMAGE_TYPE_MASK)==TGA_IMAGE_TYPE_NO_IMAGE || (sTGAHeader.imageType&TGA_IMAGE_TYPE_MASK)==TGA_IMAGE_TYPE_GRAYSCALE) return(NULL);//ãðàäàöèè ñåðîãî è îòñóòñòâèå èçîáðàæåíèÿ íå ïîääåðæèâàåì
        
    img_width = sTGAHeader.width;
    img_height = sTGAHeader.height;
    img_channels = sTGAHeader.bits/8;

    int32_t line_length=sTGAHeader.width*sizeof(uint32_t);
    
    int32_t image_length=sTGAHeader.width*sTGAHeader.height*sTGAHeader.bits/8;
    
    uint32_t image_offset=sizeof(struct STGAHeader)+sTGAHeader.colorMapStart+sTGAHeader.colorMapLength*sTGAHeader.colorMapBits/8+sTGAHeader.identsize;
    if (image_offset+image_length > file_size) return(NULL);
    
    if (sTGAHeader.bits==24) { //BGR
        data=new uint8_t[sTGAHeader.width*sTGAHeader.height*sizeof(uint32_t)];
        int32_t y,x;
        if (sTGAHeader.descriptor==TGA_DESCRIPTOR_FORWARD) {
            uint8_t *oi_ptr=data;
            uint8_t *i_ptr=data_ptr+image_offset;
            for(y=0;y<sTGAHeader.height;y++,i_ptr+=sTGAHeader.width*3) {
                uint8_t *i_ptrc=i_ptr;
                for(x=0;x<sTGAHeader.width;x++) {
                    uint8_t b=*(i_ptrc);i_ptrc++;
                    uint8_t g=*(i_ptrc);i_ptrc++;
                    uint8_t r=*(i_ptrc);i_ptrc++;
                    uint8_t a=1;
                    *oi_ptr=b;oi_ptr++;
                    *oi_ptr=g;oi_ptr++;
                    *oi_ptr=r;oi_ptr++;
                    *oi_ptr=a;oi_ptr++;
                }
            }
        }

        if (sTGAHeader.descriptor==TGA_DESCRIPTOR_REVERSE) {
            uint8_t *oi_ptr=data;
            uint8_t *i_ptr=data_ptr+image_offset+sTGAHeader.width*sTGAHeader.height*3-1;

            for(y=sTGAHeader.height-1;y>=0;y--,i_ptr-=sTGAHeader.width*3) {
                uint8_t *i_ptrc=i_ptr;

                for(x=0;x<sTGAHeader.width;x++) {
                    uint8_t b=*(i_ptrc);i_ptrc++;
                    uint8_t g=*(i_ptrc);i_ptrc++;
                    uint8_t r=*(i_ptrc);i_ptrc++;
                    uint8_t a=1;
                    *oi_ptr=b;oi_ptr++;
                    *oi_ptr=g;oi_ptr++;
                    *oi_ptr=r;oi_ptr++;
                    *oi_ptr=a;oi_ptr++;
                }
            }
        }
    }

    if (sTGAHeader.bits==32) {
        data=new uint8_t[sTGAHeader.width*sTGAHeader.height*sizeof(uint32_t)];
        int32_t y,x;

        if (sTGAHeader.descriptor==TGA_DESCRIPTOR_FORWARD) {
            uint8_t *oi_ptr=data;
            uint8_t *i_ptr=data_ptr+image_offset;

            for(y=0;y<sTGAHeader.height;y++,i_ptr+=sTGAHeader.width*sizeof(uint32_t)) {
                uint8_t *i_ptrc=i_ptr;

                for(x=0;x<sTGAHeader.width;x++) {
                    uint8_t b=*(i_ptrc);i_ptrc++;
                    uint8_t g=*(i_ptrc);i_ptrc++;
                    uint8_t r=*(i_ptrc);i_ptrc++;
                    uint8_t a=*(i_ptrc);i_ptrc++;
                    *oi_ptr=b;oi_ptr++;
                    *oi_ptr=g;oi_ptr++;
                    *oi_ptr=r;oi_ptr++;
                    *oi_ptr=a;oi_ptr++;
                }
            }
        }

        if (sTGAHeader.descriptor==TGA_DESCRIPTOR_REVERSE) {
            uint8_t *oi_ptr=data;
            uint8_t *i_ptr=data_ptr+image_offset+sTGAHeader.width*sTGAHeader.height*3-1;

            for(y=sTGAHeader.height-1;y>=0;y--,i_ptr-=sTGAHeader.width*sizeof(uint32_t)) {
                uint8_t *i_ptrc=i_ptr;

                for(x=0;x<sTGAHeader.width;x++) {
                    uint8_t b=*(i_ptrc);i_ptrc++;
                    uint8_t g=*(i_ptrc);i_ptrc++;
                    uint8_t r=*(i_ptrc);i_ptrc++;
                    uint8_t a=*(i_ptrc);i_ptrc++;
                    *oi_ptr=b;oi_ptr++;
                    *oi_ptr=g;oi_ptr++;
                    *oi_ptr=r;oi_ptr++;
                    *oi_ptr=a;oi_ptr++;
                }
            }
        }
    }

    if (sTGAHeader.colorMapType==TGA_COLOR_MAP_PALETTE && sTGAHeader.bits/8==3) {
        uint32_t palette_offset=sizeof(STGAHeader)+sTGAHeader.colorMapStart+sTGAHeader.identsize;

        if (palette_offset+sTGAHeader.colorMapLength*3 > file_size) return(NULL);

        uint8_t *color_map=data_ptr+palette_offset;
        
        data=new uint8_t[sTGAHeader.width*sTGAHeader.height*sizeof(uint32_t)];
        int32_t y,x;

        if (sTGAHeader.descriptor==TGA_DESCRIPTOR_FORWARD) {
            uint8_t *oi_ptr=data;
            uint8_t *i_ptr=data_ptr+image_offset;

            for(y=0;y<sTGAHeader.height;y++,i_ptr+=sTGAHeader.width) {
                uint8_t *i_ptrc=i_ptr;

                for(x=0;x<sTGAHeader.width;x++,i_ptrc++) {
                    int32_t index=(*i_ptrc)*3;
                    uint8_t b=color_map[index];
                    uint8_t g=color_map[index+1];
                    uint8_t r=color_map[index+2];
                    uint8_t a=1;
                    *oi_ptr=b;oi_ptr++;
                    *oi_ptr=g;oi_ptr++;
                    *oi_ptr=r;oi_ptr++;
                    *oi_ptr=a;oi_ptr++;
                }
            }
        }

        if (sTGAHeader.descriptor==TGA_DESCRIPTOR_REVERSE) {
            uint8_t *oi_ptr=data;
            uint8_t *i_ptr=data_ptr+image_offset+sTGAHeader.width*(sTGAHeader.height-1);

            for(y=sTGAHeader.height-1;y>=0;y--,i_ptr-=sTGAHeader.width) {
                uint8_t *i_ptrc=i_ptr;

                for(x=0;x<sTGAHeader.width;x++,i_ptrc++) {
                    int32_t index=(*i_ptrc)*3;
                    uint8_t b=color_map[index];
                    uint8_t g=color_map[index+1];
                    uint8_t r=color_map[index+2];
                    uint8_t a=1;
                    *oi_ptr=b;oi_ptr++;
                    *oi_ptr=g;oi_ptr++;
                    *oi_ptr=r;oi_ptr++;
                    *oi_ptr=a;oi_ptr++;
                }
            }
        }
    }

    return true;
}
*/
