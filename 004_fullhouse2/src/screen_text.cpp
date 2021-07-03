
#include <iostream>
#include <string>

#include "screen_text.h"
#include "common.h"
#include "bitmap.h"
#include <fmt/format.h>

CScreenText::~CScreenText() {
    glDeleteTextures(1, &font);
}

void CScreenText::loadFont(const std::string& fname) {
    CJpegBitmap fontImage;

    if (fontImage.readFromFile(fname)) {
        std::cout << "CScreenText::loadFont(): load font - " << fname << "\n";
        std::cout << fmt::format("  Font parameters: Width - {}, Height - {}\n", fontImage.getWidht(), fontImage.getHeight());
    } else {
        std::cout << "CScreenText::loadFont(): can't load font from file - " << fname << "\n!";
        return;
    }

    fontXRes = static_cast<float>(fontImage.getWidht());
    fontYRes = static_cast<float>(fontImage.getHeight());
    // Размер ячейки с символом получается делением горизонтального и вертикального 
    // размера изображения на количество столбцов и строк соответственно.
    fontCellXSize = fontXRes/fontColumn;
    fontCellYSize = fontYRes/fontRow;

    glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &font);
	glBindTexture(GL_TEXTURE_2D, font);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, fontImage.getWidht(), fontImage.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, fontImage.getDataPtr());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void CScreenText::setGlyphWidth(float size) {
    glyphQuadWidth = size;
}

void CScreenText::setGlyphHeight(float size) {
    glyphQuadHeight = size;
}

void CScreenText::setGlyphGap(float size) {
    glyphQuadGap = size;
}

void CScreenText::setTextPosition(float x, float y) {
    textPosX = x;
    textPosY = y;
}

void CScreenText::drawString(std::string string) {
    float offset; // Смещение квада с i-ым символом, зависит от ширины квадов и зазора между ними
    int32_t glyph_x; // Столбец, в котором находится символ 
    int32_t glyph_y; // Строка, в котором находится символ 
    float tc_gap_x = 1.0f / fontColumn; // Размер ячейки с символом в долях текстурных координат по горизонтали
    float tc_gap_y = 1.0f / fontRow;    // Размер ячейки с символом в долях текстурных координат по вертикали

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
	glBindTexture(GL_TEXTURE_2D, font);

    for (size_t i = 0; i < string.length(); i++) {
        offset = (glyphQuadWidth+glyphQuadGap)*i;

        glyph_x = string[i]%fontColumn;
        glyph_y = (string[i]/fontColumn)-1;

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
        glTexCoord2f((tc_gap_x*glyph_x),          (tc_gap_y*glyph_y)+0.0f);    glVertex2f((offset + 0.0f) + textPosX,           0.0f + textPosY);
        // Правый верхний угол
        glTexCoord2f((tc_gap_x*glyph_x)+tc_gap_x, (tc_gap_y*glyph_y)+0.0f);    glVertex2f((offset + glyphQuadWidth) + textPosX, 0.0f + textPosY);
        // Правый гижний угол
        glTexCoord2f((tc_gap_x*glyph_x)+tc_gap_x, (tc_gap_y*glyph_y)+tc_gap_y); glVertex2f((offset + glyphQuadWidth) + textPosX, -glyphQuadHeight + textPosY);
        // Левый нижний угол
        glTexCoord2f((tc_gap_x*glyph_x),          (tc_gap_y*glyph_y)+tc_gap_y); glVertex2f((offset + 0.0f) + textPosX,           -glyphQuadHeight + textPosY);
        glEnd();
    }
}
