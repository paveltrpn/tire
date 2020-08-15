
#ifndef __bitmap_text_h__
#define __bitmap_text_h__

#include <iostream>
#include <string>
#include <vector>

#include "algebra2.h"

struct glyph_quad_s {
    vec2_t a,b,c,d;
    vec2_t ta,tb,tc,td;
};

class bitmap_text_c {
    private:
        float   glyph_quad_wdt = 2.0f;
        float   glyph_quad_hgt = 2.0f;
        float   glyph_quad_gap = -0.5f;

        float   text_pos_x = 0.0f;
        float   text_pos_y = 0.0f;

        // Формат шрифта - изображение TGA с началом сверху слева.
        // Разрешение - 2048х512, размер ячейки с символом 64х64.
        // 32 символа в строке на 8 строк символов.
        float   font_xres = 2048.0f;
        float   font_yres = 512.0f;
        float   font_cell_xsize = 64.0f;
        float   font_cell_ysize = 64.0f;

    public:
        GLuint  font;

        bitmap_text_c();
        ~bitmap_text_c();

        void load_font(std::string fname);
        
        void set_glyph_width(float size);
        void set_glyph_height(float size);
        void set_glyph_gap(float size);

        void set_text_position(float x, float y);

        void draw_string(std::string string);      
};

#endif