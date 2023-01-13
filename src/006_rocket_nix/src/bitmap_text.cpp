
#include <iostream>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include "GLFW/glfw3.h"

#include "bitmap_text.h"
#include "common.h"
#include "bitmap.h"

bitmap_text_c::~bitmap_text_c() {
    glDeleteTextures(1, &font);
}

void bitmap_text_c::load_font(std::string fname) {
    bitmap_c image;

    image.load_tga(fname);
    image.show_info();

    font_xres = (float)image.get_widht();
    font_yres = (float)image.get_height();
    // Размер ячейки с символом получается делением горизонтального и вертикального 
    // размера изображения на количество столбцов и строк соответственно.
    font_cell_xsize = font_xres/font_column;
    font_cell_ysize = font_yres/font_row;

    glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &font);
	glBindTexture(GL_TEXTURE_2D, font);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image.get_widht(), image.get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.get_data_ptr());
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
    float offset; // Смещение квада с i-ым символом, зависит от ширины квадов и зазора между ними
    int32_t glyph_x; // Столбец, в котором находится символ 
    int32_t glyph_y; // Строка, в котором находится символ 
    float tc_gap_x = 1.0f / font_column; // Размер ячейки с символом в долях текстурных координат по горизонтали
    float tc_gap_y = 1.0f / font_row;    // Размер ячейки с символом в долях текстурных координат по вертикали

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
	glBindTexture(GL_TEXTURE_2D, font);

    for (size_t i = 0; i < string.length(); i++) {
        offset = (glyph_quad_wdt+glyph_quad_gap)*i;

        glyph_x = string[i]%font_column;
        glyph_y = (string[i]/font_column)-1;

        // Текстурные координаты (или ша, размер ячейки в долях текстурных координат) 
        // рассчитываются как соотношение размера ячейки с символом к размеру изображения
        // tc_gap_x = font_cell_xsize / font_xres;
        // tc_gap_y = font_cell_ysize / font_yres;
        // Или, зная количество ячеек с символами (количество столбцов и строк, 32 на 8):
        // tc_gap_x = 1.0f / font_column;
        // tc_gap_y = 1.0f / font_row;

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
        // Левый верхний угол
        glTexCoord2f((tc_gap_x*glyph_x),          (tc_gap_y*glyph_y)+0.0f);    glVertex2f((offset + 0.0f) + text_pos_x,           0.0f + text_pos_y);
        // Правый верхний угол
        glTexCoord2f((tc_gap_x*glyph_x)+tc_gap_x, (tc_gap_y*glyph_y)+0.0f);    glVertex2f((offset + glyph_quad_wdt) + text_pos_x, 0.0f + text_pos_y);
        // Правый гижний угол
        glTexCoord2f((tc_gap_x*glyph_x)+tc_gap_x, (tc_gap_y*glyph_y)+tc_gap_y); glVertex2f((offset + glyph_quad_wdt) + text_pos_x, -glyph_quad_hgt + text_pos_y);
        // Левый нижний угол
        glTexCoord2f((tc_gap_x*glyph_x),          (tc_gap_y*glyph_y)+tc_gap_y); glVertex2f((offset + 0.0f) + text_pos_x,           -glyph_quad_hgt + text_pos_y);
        glEnd();
    }
}
