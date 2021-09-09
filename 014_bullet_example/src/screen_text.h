
#ifndef __screen_text_h__
#define __screen_text_h__

#include <iostream>
#include <string>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>

class CScreenText {
    private:
        float   glyphQuadWidth;
        float   glyphQuadHeight;
        float   glyphQuadGap;

        float   textPosX;
        float   textPosY;

        // Формат шрифта - изображение с началом сверху слева,
        // 32 столбца на 8 строк символов, первый символ - 32 ("пробел").
        // Размер ячейки с символом получается делением горизонтального и вертикального 
        // размера изображения на количество столбцов и строк соответственно.
        int32_t fontColumn = 32; // Количество столбов символов в шрифте
        int32_t fontRow = 8;     // Количество строк символов в шрифте

        float   fontXRes;
        float   fontYRes;
        float   fontCellXSize;
        float   fontCellYSize;

        // Запрещаем копирование и перемещение
        CScreenText(CScreenText& other) = delete;
        CScreenText(CScreenText&& other) = delete;
        CScreenText& operator=(CScreenText &other) = delete;
        CScreenText& operator=(CScreenText &&other) = delete;

    public:
        GLuint  font;

        CScreenText();
        // Инициализирующий конструктор лучше не использовать т.к. он может быть вызван до создания 
        // контекста OpenGL и это приведёт к segmentation fault в функции создания текстур (gluBuild2DMipmaps() или
        // glGenerateMipmap()). Лучшая стратегия здесь - это запретить конструктор и оператор копирования и перемещения,
        // и ограничиться только конструктором по умолчанию, не инициализирующим объект,
        // что-бы объект этого класса, будучи членом другого класса мог быть инициализирован только явным
        // вызовом функции (например load_tga()).
        //
        // CScreenText(string fname);
        ~CScreenText();
        
        void loadFromFile(const std::string& fname);

        void setGlyphWidth(float size);
        void setGlyphHeight(float size);
        void setGlyphGap(float size);

        void setTextPosition(float x, float y);

        void drawString(std::string string);      
};

#endif