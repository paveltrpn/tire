
#include "tga.h"

/*
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

static uint8_t *GetTGAImage(uint8_t *data_ptr,uint32_t length,int32_t &width,int32_t &height)
{
 STGAHeader sTGAHeader;
 
 if (length<sizeof(STGAHeader)) return(NULL);
 memcpy(&sTGAHeader,data_ptr,sizeof(STGAHeader));
 //ïðîâåðÿåì íà âîçìîæíîñòü ÷òåíèÿ
 if (sTGAHeader.imageType&TGA_IMAGE_TYPE_RLE_MASK) return(NULL);//RLE íå ïîääåðæèâàåì
 if ((sTGAHeader.imageType&TGA_IMAGE_TYPE_MASK)==TGA_IMAGE_TYPE_NO_IMAGE || (sTGAHeader.imageType&TGA_IMAGE_TYPE_MASK)==TGA_IMAGE_TYPE_GRAYSCALE) return(NULL);//ãðàäàöèè ñåðîãî è îòñóòñòâèå èçîáðàæåíèÿ íå ïîääåðæèâàåì
 //çàäà¸ì ïàðàìåòðû èçîáðàæåíèÿ
 width=sTGAHeader.width;
 height=sTGAHeader.height;
 int32_t line_length=sTGAHeader.width*sizeof(uint32_t);
 //âûäåëÿåì ïàìÿòü äëÿ èçîáðàæåíèÿ
 int32_t image_length=sTGAHeader.width*sTGAHeader.height*sTGAHeader.bits/8;
 //ñ÷èòûâàåì èçîáðàæåíèå
 uint32_t image_offset=sizeof(struct STGAHeader)+sTGAHeader.colorMapStart+sTGAHeader.colorMapLength*sTGAHeader.colorMapBits/8+sTGAHeader.identsize;
 if (image_offset+image_length>length) return(NULL);//íåäîñòàòî÷íî äàííûõ
 //à òåïåðü àíàëèçèðóåì ôîðìàò
 if (sTGAHeader.bits==24)//BGR - ìîäèöèôèðóåì äëÿ ÷åòâ¸ðîê áàéò
 {
  uint8_t *out_image=new uint8_t[sTGAHeader.width*sTGAHeader.height*sizeof(uint32_t)];
  int32_t y,x;
  if (sTGAHeader.descriptor==TGA_DESCRIPTOR_FORWARD)//ïðÿìîé ôîðìàò
  {
   uint8_t *oi_ptr=out_image;
   uint8_t *i_ptr=data_ptr+image_offset;
   for(y=0;y<sTGAHeader.height;y++,i_ptr+=sTGAHeader.width*3)
   {
    uint8_t *i_ptrc=i_ptr;
    for(x=0;x<sTGAHeader.width;x++)
    {
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
  if (sTGAHeader.descriptor==TGA_DESCRIPTOR_REVERSE)//îáðàòíûé ôîðìàò
  {
   uint8_t *oi_ptr=out_image;
   uint8_t *i_ptr=data_ptr+image_offset+sTGAHeader.width*sTGAHeader.height*3-1;
   for(y=sTGAHeader.height-1;y>=0;y--,i_ptr-=sTGAHeader.width*3)
   {
    uint8_t *i_ptrc=i_ptr;
    for(x=0;x<sTGAHeader.width;x++)
    {
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
  return(out_image);
 }

 if (sTGAHeader.bits==32)//BGR - ìîäèöèôèðóåì äëÿ ÷åòâ¸ðîê áàéò
 {
  uint8_t *out_image=new uint8_t[sTGAHeader.width*sTGAHeader.height*sizeof(uint32_t)];
  int32_t y,x;
  if (sTGAHeader.descriptor==TGA_DESCRIPTOR_FORWARD)//ïðÿìîé ôîðìàò
  {
   uint8_t *oi_ptr=out_image;
   uint8_t *i_ptr=data_ptr+image_offset;
   for(y=0;y<sTGAHeader.height;y++,i_ptr+=sTGAHeader.width*sizeof(uint32_t))
   {
    uint8_t *i_ptrc=i_ptr;
    for(x=0;x<sTGAHeader.width;x++)
    {
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
  if (sTGAHeader.descriptor==TGA_DESCRIPTOR_REVERSE)//îáðàòíûé ôîðìàò
  {
   uint8_t *oi_ptr=out_image;
   uint8_t *i_ptr=data_ptr+image_offset+sTGAHeader.width*sTGAHeader.height*3-1;
   for(y=sTGAHeader.height-1;y>=0;y--,i_ptr-=sTGAHeader.width*sizeof(uint32_t))
   {
    uint8_t *i_ptrc=i_ptr;
    for(x=0;x<sTGAHeader.width;x++)
    {
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
  return(out_image);
 }

 if (sTGAHeader.colorMapType==TGA_COLOR_MAP_PALETTE && sTGAHeader.colorMapBits/8==3)//åñòü ïàëèòðà ïî 24 áèòà (äðóãóþ ïàëèòðó íå ïîääåðæèâàåì)
 {
  uint32_t palette_offset=sizeof(STGAHeader)+sTGAHeader.colorMapStart+sTGAHeader.identsize;
  if (palette_offset+sTGAHeader.colorMapLength*3>length) return(NULL);//íå õâàòàåò äàííûõ
  uint8_t *color_map=data_ptr+palette_offset;
  //íàì ïîòðåáóåòñÿ èçìåíèòü ôîðìàò
  uint8_t *out_image=new uint8_t[sTGAHeader.width*sTGAHeader.height*sizeof(uint32_t)];
  int32_t y,x;
  if (sTGAHeader.descriptor==TGA_DESCRIPTOR_FORWARD)//ïðÿìîé ôîðìàò
  {
   uint8_t *oi_ptr=out_image;
   uint8_t *i_ptr=data_ptr+image_offset;
   for(y=0;y<sTGAHeader.height;y++,i_ptr+=sTGAHeader.width)
   {
    uint8_t *i_ptrc=i_ptr;
    for(x=0;x<sTGAHeader.width;x++,i_ptrc++)
    {
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
  if (sTGAHeader.descriptor==TGA_DESCRIPTOR_REVERSE)//ôîðìàò ïåðåâ¸ðíóòûé
  {
   uint8_t *oi_ptr=out_image;
   uint8_t *i_ptr=data_ptr+image_offset+sTGAHeader.width*(sTGAHeader.height-1);
   for(y=sTGAHeader.height-1;y>=0;y--,i_ptr-=sTGAHeader.width)
   {
    uint8_t *i_ptrc=i_ptr;
    for(x=0;x<sTGAHeader.width;x++,i_ptrc++)
    {
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
  return(out_image);
 }
 //èíûå ðåæèìû íå ïîääåðæèâàåì
 return(NULL);
}


//----------------------------------------------------------------------------------------------------
//çàãðóçèòü tga-ôàéë
//----------------------------------------------------------------------------------------------------
uint8_t *LoadTGAFromFile(const char *file_name,int32_t &width,int32_t &height)
{
 CRAIIFileIn cRAIIFileIn(file_name,std::ios_base::in|std::ios_base::binary);
 {
  if (cRAIIFileIn.IsOpened()==false) return(NULL);
  //óçíàåì ðàçìåð ôàéëà
  cRAIIFileIn.GetHandle().seekg(0,std::ios_base::end);
  uint32_t file_size=static_cast<uint32_t>(cRAIIFileIn.GetHandle().tellg());
  cRAIIFileIn.GetHandle().seekg(0,std::ios_base::beg);
  //÷èòàåì ôàéë
  uint8_t *data_ptr=new uint8_t[file_size+1];
  if (cRAIIFileIn.GetHandle().read(reinterpret_cast<char*>(data_ptr),sizeof(uint8_t)*file_size).fail()==false)
  {
   uint8_t *ret=GetTGAImage(data_ptr,file_size,width,height);
   delete[](data_ptr);
   return(ret);
  }
  delete[](data_ptr);
 }
 return(NULL);
}

//----------------------------------------------------------------------------------------------------
//çàãðóçèòü tga-ôàéë èç ðåñóðñîâ
//----------------------------------------------------------------------------------------------------
uint8_t *LoadTGAFromResource(HMODULE hModule,int32_t id,int32_t &width,int32_t &height)
{
 HRSRC hRSRC=FindResource(hModule,(LPSTR)id,RT_RCDATA);
 if (hRSRC==NULL) return(NULL);
 HGLOBAL hGlobal=LoadResource(hModule,hRSRC);
 if (hGlobal==NULL) return(NULL);
 uint8_t *data_ptr=(uint8_t*)LockResource(hGlobal);
 uint32_t data_size=SizeofResource(hModule,hRSRC);
 //÷èòàåì äàííûå
 uint8_t *ret=GetTGAImage(data_ptr,data_size,width,height);
 GlobalUnlock(hGlobal);
 return(ret);
}
//----------------------------------------------------------------------------------------------------
//ñîõðàíèòü êàðòèíêó â tga-ôàéë
//----------------------------------------------------------------------------------------------------
bool SaveTGA(const char *file_name,int32_t width,int32_t height,uint8_t *image)
{
 struct STGAHeader sTGAHeader;
 sTGAHeader.identsize=0;
 sTGAHeader.colorMapType=0;
 sTGAHeader.imageType=2;
 sTGAHeader.colorMapStart=0;
 sTGAHeader.colorMapLength=0;
 sTGAHeader.colorMapBits=24;
 sTGAHeader.xstart=0;
 sTGAHeader.ystart=0;
 sTGAHeader.width=static_cast<uint16_t>(width);
 sTGAHeader.height=static_cast<uint16_t>(height);
 sTGAHeader.bits=32;
 sTGAHeader.descriptor=32;
 CRAIIFileOut cRAIIFileOut(file_name,std::ios_base::out|std::ios_base::binary);
 if (cRAIIFileOut.IsOpened()==false) return(false);
 if (cRAIIFileOut.GetHandle().write(reinterpret_cast<char*>(&sTGAHeader),sizeof(STGAHeader)).fail()==true) return(false);
 if (cRAIIFileOut.GetHandle().write(reinterpret_cast<char*>(image),sizeof(uint8_t)*width*height*4).fail()==true) return(false);
 return(true);
}*/