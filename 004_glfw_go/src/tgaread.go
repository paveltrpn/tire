package main

import (
	"fmt"
	"os"
)

/*
TGA *
TGAOpen(const char *file,
	const char *mode)
{
 	TGA *tga = (TGA*) malloc(sizeof(TGA));
	if (!tga) {
		TGA_ERROR(tga, TGA_OOM);
		return NULL;
	}

	FILE *fd = fopen(file, mode);
	if (!fd) {
		TGA_ERROR(tga, TGA_OPEN_FAIL);
		free(tga);
		return NULL;
	}

	tga->fd = fd;
	tga->off = 0;
	bzero(&tga->hdr, sizeof(TGAHeader));
	tga->last = TGA_OK;
	tga->error = (TGAErrorProc) 0;
	return tga;
}

void
TGAClose(TGA *tga)
{
	if (tga) {
		fclose(tga->fd);
		free(tga);
	}
}

size_t
TGARead(TGA    *tga,
	tbyte  *buf,
	size_t 	size,
	size_t 	n)
{
	size_t read = fread(buf, size, n, tga->fd);
	if (read != n) {
		TGA_ERROR(tga, TGA_READ_FAIL);
	}
	tga->off = ftell(tga->fd);
	return read;
}

tlong
__TGASeek(TGA  *tga,
	  tlong off,
	  int   whence)
{
	fseek(tga->fd, off, whence);
	long offset = ftell(tga->fd);
	if (offset == -1) {
		TGA_ERROR(tga, TGA_SEEK_FAIL);
	}
	tga->off = offset;
	return tga->off;
}
*/

func tgaOpen(file string) (tga TGA) {
	fd, err := os.Open(file)
	if err != nil {
		fmt.Println("tgaOpen(): can't open file!")
	}

	tga.fd = fd
	tga.off = 0

	return tga
}

func tgaClose(tga *TGA) {
	tga.fd.Close()
}

func tgaRead(tga *TGA, size int, n int) ([]uint8, int) {
	buf := make([]uint8, size*n)

	bRead, err := tga.fd.Read(buf)
	if err != nil {
		// handle error
	}

	return buf, bRead
}

// Действует аналогично fseek(FILE *stream, long offset, int whence)
// whence может быть -
// SEEK_SET	Начало файла		- 0*
// SEEK_CUR	Текущая позиция		- 1*
// SEEK_END	Конец файла			- 2*
// * - для os.File.Seek(offset int64, whence int) (ret int64, err error)
func tgaSeek(tga *TGA, off int64, whence int) tlong {
	offset, err := tga.fd.Seek(off, whence)
	if err != nil {
		// handle error
	}

	tga.off = tlong(offset)

	return tga.off
}

func tgaReadHeader(tga *TGA) {
	var (
		tmp []uint8
	)

	tgaSeek(tga, 0, 0)

	tmp, _ = tgaRead(tga, tgaHeaderSize, 1)

	tga.hdr.idLen = tmp[0]
	tga.hdr.mapType = tmp[1]
	tga.hdr.imgType = tmp[2]
	tga.hdr.mapFirst = tshort(tmp[3]) + (tshort(tmp[4]))*256
	tga.hdr.mapLen = tshort(tmp[5]) + (tshort(tmp[6]))*256
	tga.hdr.mapEntry = tmp[7]
	tga.hdr.x = tshort(tmp[8]) + (tshort(tmp[9]))*256
	tga.hdr.y = tshort(tmp[10]) + (tshort(tmp[11]))*256
	tga.hdr.width = tshort(tmp[12]) + (tshort(tmp[13]))*256
	tga.hdr.height = tshort(tmp[14]) + (tshort(tmp[15]))*256
	tga.hdr.depth = tmp[16]
	tga.hdr.alpha = tmp[17] & 0x0f

	if (tmp[17] & 0x10) == 1 {
		tga.hdr.horz = tgaRIGHT
	} else {
		tga.hdr.horz = tgaLEFT
	}

	if (tmp[17] & 0x20) == 1 {
		tga.hdr.vert = tgaTOP
	} else {
		tga.hdr.vert = tgaBOTTOM
	}

	if ((tgaIsMapped(tga)) && (tga.hdr.depth != 8)) == true {
		fmt.Println("tgaReadHeader(): TGA_UNKNOWN_SUB_FORMAT!")
	}

	if ((tga.hdr.depth != 8) &&
		(tga.hdr.depth != 15) &&
		(tga.hdr.depth != 16) &&
		(tga.hdr.depth != 24) &&
		(tga.hdr.depth != 32)) == true {
		fmt.Println("tgaReadHeader(): unknown TGA bit depth!")
	}

}
