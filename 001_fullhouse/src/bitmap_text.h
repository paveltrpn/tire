
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
        // 32 столбца на 8 строк символов, первый символ - 32 ("пробел").
        // Размер ячейки с символом получается делением горизонтального и вертикального 
        // размера изображения на количество столбцов и строк соответственно.
        int32_t font_column = 32; // Количество столбов символов в шрифте
        int32_t font_row = 8;     // Количество строк символов в шрифте

        float   font_xres;
        float   font_yres;
        float   font_cell_xsize;
        float   font_cell_ysize;

        // Запрещаем копирование и перемещение
        bitmap_text_c(bitmap_text_c& other) = delete;
        bitmap_text_c(bitmap_text_c&& other) = delete;
        bitmap_text_c& operator=(bitmap_text_c &other) = delete;
        bitmap_text_c& operator=(bitmap_text_c &&other) = delete;

    public:
        GLuint  font;

        bitmap_text_c() {};
        // Инициализирующий конструктор лучше не использовать т.к. он может быть вызван до создания 
        // контекста OpenGL и это приведёт к segmentation fault в функции создания текстур (gluBuild2DMipmaps() или
        // glGenerateMipmap()). Лучшая стратегия здесь - это запретить конструктор и оператор копирования и перемещения,
        // и ограничиться только конструктором по умолчанию, не инициализирующим объект,
        // что-бы объект этого класса, будучи членом другого класса мог быть инициализирован только явным
        // вызовом функции (например load_tga()).
        //
        // bitmap_text_c(string fname);
        ~bitmap_text_c();
        
        void load_font(std::string fname);
        
        void set_glyph_width(float size);
        void set_glyph_height(float size);
        void set_glyph_gap(float size);

        void set_text_position(float x, float y);

        void draw_string(std::string string);      
};

#endif