package main

import (
	"fmt"
	"io"
	"os"
)

// Основано на https://github.com/madebr/libtga

const (
	tgaImageID   = 0x01
	tgaImageInfo = 0x02
	tgaImageData = 0x04
	tgaColorMap  = 0x08
)

const (
	tgaRLEEncode = 0x10
)

const (
	tgaRGB = 0x20
	tgaBGR = 0x40
)

const (
	tgaBOTTOM = 0x0
	tgaTOP    = 0x1
	tgaLEFT   = 0x0
	tgaRIGHT  = 0x1
)

const (
	tgaIMGTypeNOIMAGE     = 0
	tgaIMGTypeUNCOMPCMAP  = 1
	tgaIMGTypeUNCOMPTRUEC = 2
	tgaIMGTypeUNCOMPBW    = 3
	tgaIMGTypeRLECMAP     = 9
	tgaIMGTypeRLETRUEC    = 10
	tgaIMGTypeRLEBW       = 11
)

const (
	tgaIMGTypeCMAPFLAG  = 0x1
	tgaIMGTypeTRUECFLAG = 0x2
	tgaIMGTypeBWFLAG    = 0x3
)

const (
	tgaHeaderSize = 18
)

type tuint32 uint32
type tuint16 uint16
type tuint8 uint8

type tbyte tuint8
type tshort tuint16
type tlong tuint32

type TGAHeader struct {
	idLen    uint8  /* F1: image id length */
	mapType  uint8  /* F2: color map type */
	imgType  uint8  /* F3: image type */
	mapFirst tshort /* F4.1: index of first map entry */
	mapLen   tshort /* F4.2: number of entries in color map */
	mapEntry uint8  /* F4.3: bit-depth of a cmap entry */
	x        tshort /* F5.1: x-coordinate */
	y        tshort /* F5.2: y-coordinate */
	width    tshort /* F5.3: width of image */
	height   tshort /* F5.4: height of image */
	depth    uint8  /* F5.5: pixel-depth of image */
	alpha    uint8  /* F5.6 (bits 3-0): alpha bits */
	horz     uint8  /* F5.6 (bit 4): horizontal orientation */
	vert     uint8  /* F5.6 (bit 5): vertical orientation */
}

type TGAData struct {
	imgID   []uint8 /* F6: image id */
	cmap    *uint8  /* F7: color map */
	imgData *uint8  /* F8: image data */
	flags   tuint32
}

type TGA struct {
	fd  *os.File /* file stream */
	off tlong    /* current offset in file*/
	// last int       /* last error code */
	hdr TGAHeader /* image header */
	// error TGAErrorProc /* user-defined error proc */
}

func loadFromTGA(fname string) {
	var (
		tga    TGA
		inData TGAData
	)

	tga = tgaOpen(fname)
	defer tgaClose(&tga)

	inData.flags = tgaImageID | tgaImageData | tgaRGB

	tgaReadHeader(&tga)

	fmt.Println(tga.hdr.width)
	fmt.Println(tga.hdr.height)
	fmt.Println(tga.hdr.depth)
}

func ftell(file *os.File) int64 {
	offset, err := file.Seek(0, io.SeekCurrent)
	if err != nil {
		// handle error
	}

	return offset
}

func tgaCmapSize(tga *TGA) tshort {
	return (tga.hdr.mapLen * tshort(tga.hdr.mapEntry/8))
}

func tgaCmapOFF(tga *TGA) uint8 {
	return (tgaHeaderSize + tga.hdr.idLen)
}

func tgaIMGDataOFF(tga *TGA) tshort {
	return (tgaHeaderSize + (tshort(tga.hdr.idLen) + tgaCmapSize(tga)))
}

func tgaIMGDataSize(tga *TGA) tshort {
	return (tga.hdr.width * tga.hdr.height * tshort(tga.hdr.depth/8))
}

func tgaScanlineSize(tga *TGA) tshort {
	return (tga.hdr.width * tshort(tga.hdr.depth/8))
}

func tgaCanSwap(depth tbyte) bool {
	return (depth == 24 || depth == 32)
}

func tgaIMGTypeAvailable(tga *TGA) bool {
	return ((tga.hdr.imgType) != 0)
}

func tgaIMGTypeISMapped(tga *TGA) bool {
	return (((tga.hdr.imgType) & 0x3) == 0x1)
}

func tgaIMGTypeISTrueC(tga *TGA) bool {
	return (((tga.hdr.imgType) & 0x3) == 0x2)
}

func tgaIMGTypeISBW(tga *TGA) bool {
	return (((tga.hdr.imgType) & 0x3) == 0x3)
}

func tgaIMGTypeISEencoded(tga *TGA) bool {
	return (((tga.hdr.imgType) & 0x8) == 0x8)
}

func tgaHasID(tga *TGA) bool {
	return (tga.hdr.idLen != 0)
}

func tgaIsMapped(tga *TGA) bool {
	return (tga.hdr.mapType == 1)
}
