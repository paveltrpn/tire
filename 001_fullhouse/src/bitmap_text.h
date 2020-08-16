
#ifndef __bitmap_text_h__
#define __bitmap_text_h__

#include <iostream>
#include <string>
#include <vector>

#include "algebra2.h"

struct glyph_quad_s {
    vec2_t lu, ru, rd, ld;
    vec2_t tclu,tcru,tcrd,tcld;
};

class bitmap_text_c {
    private:
        float   glyph_quad_wdt = 0.5f;
        float   glyph_quad_hgt = 1.0f;
        float   glyph_quad_gap = 0.0f;

        float   text_pos_x = 0.0f;
        float   text_pos_y = 0.0f;

        // Формат шрифта - изображение TGA с началом сверху слева,
        // 32 столбца на 8 строк символов.
        // Размер ячейки с символом получается делением горизонтального и вертикального 
        // размера изображения на количество столбцов и строк соответственно.
        const int32_t font_column = 32;
        const int32_t font_row = 8;

        float   font_xres;
        float   font_yres;
        float   font_cell_xsize;
        float   font_cell_ysize;

    public:
        GLuint  font;

        bitmap_text_c() {};
        ~bitmap_text_c();
        
        void load_font(std::string fname);
        
        void set_glyph_width(float size);
        void set_glyph_height(float size);
        void set_glyph_gap(float size);

        void set_text_position(float x, float y);

        void draw_string(std::string string);      
};

#endif