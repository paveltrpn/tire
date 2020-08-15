
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GLFW/glfw3.h>

#include "bitmap_text.h"
#include "algebra2.h"
#include "bitmap.h"

bitmap_text_c::bitmap_text_c() {
}

bitmap_text_c::~bitmap_text_c() {
    glDeleteTextures(1, &font);
}

void bitmap_text_c::load_font(std::string fname) {
    tga_img_c image;

    image.from_file(fname);
    image.show_img_stats();
     
    glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &font);
	glBindTexture(GL_TEXTURE_2D, font);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image.get_widht(), image.get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.get_data());
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, tex_image.get_widht(), tex_image.get_height(), GL_RGB, GL_UNSIGNED_BYTE, tex_image.get_data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void bitmap_text_c::set_glyph_width(float size) {
    glyph_quad_wdt = size;
}

void bitmap_text_c::set_glyph_height(float size) {
    glyph_quad_hgt = size;
}

void bitmap_text_c::set_glyph_gap(float size) {
    glyph_quad_gap = size;
}

void bitmap_text_c::set_text_position(float x, float y) {
    text_pos_x = x;
    text_pos_y = y;
}

void bitmap_text_c::draw_string(std::string string) {
    size_t i;
    float offset;
    int32_t glyph_x; 
    int32_t glyph_y;
    float bf_xtex = font_cell_xsize/font_xres;
    float bf_ytex = font_cell_ysize/font_yres;

    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, font);

    for (i = 0; i < string.length(); i++) {
        offset = (glyph_quad_wdt+glyph_quad_gap)*i;

        glyph_x = string[i]%32;
        glyph_y = (string[i]/32)-1;

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
        // Левый верхний угол
        glTexCoord2f((bf_xtex*glyph_x),         (bf_ytex*glyph_y)+0.0f);    glVertex2f((offset + 0.0f) + text_pos_x,           0.0f + text_pos_y);
        // Правый верхний угол
        glTexCoord2f((bf_xtex*glyph_x)+bf_xtex, (bf_ytex*glyph_y)+0.0f);    glVertex2f((offset + glyph_quad_wdt) + text_pos_x, 0.0f + text_pos_y);
        // Правый гижний угол
        glTexCoord2f((bf_xtex*glyph_x)+bf_xtex, (bf_ytex*glyph_y)+bf_ytex); glVertex2f((offset + glyph_quad_wdt) + text_pos_x, -glyph_quad_hgt + text_pos_y);
        // Левый нижний угол
        glTexCoord2f((bf_xtex*glyph_x),         (bf_ytex*glyph_y)+bf_ytex); glVertex2f((offset + 0.0f) + text_pos_x,           -glyph_quad_hgt + text_pos_y);
        glEnd();
    }
}
