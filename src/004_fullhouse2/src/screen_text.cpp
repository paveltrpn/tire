
#include <iostream>
#include <string>
#include <array>
#include <fmt/format.h>

#include "screen_text.h"
#include "bitmap.h"

CScreenText::CScreenText() {
    constexpr float c_fontScale = 0.8;

    glyphQuadWidth = 0.5f * c_fontScale;
    glyphQuadHeight = 1.0f * c_fontScale;
    glyphQuadGap = -0.02f;
    textPosX = 0.0f;
    textPosY = 0.0f;
}

CScreenText::~CScreenText() {
    glDeleteTextures(1, &font);
}

void CScreenText::loadFromFile(const std::string& fname) {
    CBitmap fontImage;

    if (fontImage.readFromFile(fname)) {
        std::cout << "CScreenText::loadFont(): load font - " << fname << "\n";
    } else {
        std::cout << "CScreenText::loadFont(): can't load font from file - " << fname << "\n!";
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

    std::array<float, 18> v_vert{};
    std::array<float, 12> v_tcoords{};

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
	glBindTexture(GL_TEXTURE_2D, font);

    glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

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

        // Левый верхний угол
        v_vert[0] = (offset + 0.0f) + textPosX; 
        v_vert[1] = 0.0f + textPosY; 
        v_vert[2] = 0.0f;

        v_tcoords[0] = tc_gap_x*glyph_x; 
        v_tcoords[1] = (tc_gap_y*glyph_y)+0.0f;

        // Правый верхний угол
        v_vert[3] = (offset + glyphQuadWidth) + textPosX; 
        v_vert[4] = 0.0f + textPosY; 
        v_vert[5] = 0.0f;

        v_tcoords[2] = (tc_gap_x*glyph_x)+tc_gap_x; 
        v_tcoords[3] = (tc_gap_y*glyph_y)+0.0f;

        // Правый нижний угол
        v_vert[6] = (offset + glyphQuadWidth) + textPosX; 
        v_vert[7] = -glyphQuadHeight + textPosY; 
        v_vert[8] = 0.0f;

        v_tcoords[4] = (tc_gap_x*glyph_x)+tc_gap_x; 
        v_tcoords[5] = (tc_gap_y*glyph_y)+tc_gap_y;

        // Левый нижний угол
        v_vert[9] = (offset + 0.0f) + textPosX; 
        v_vert[10] = -glyphQuadHeight + textPosY; 
        v_vert[11] = 0.0f;

        v_tcoords[6] = tc_gap_x*glyph_x; 
        v_tcoords[7] = (tc_gap_y*glyph_y)+tc_gap_y;

        // Опять левый верхний
        v_vert[12] = v_vert[0]; 
        v_vert[13] = v_vert[1]; 
        v_vert[14] = v_vert[2];

        v_tcoords[8] = v_tcoords[0]; 
        v_tcoords[9] = v_tcoords[1];

        // Опять правый нижний
        v_vert[15] = v_vert[6]; 
        v_vert[16] = v_vert[7]; 
        v_vert[17] = v_vert[8];

        v_tcoords[10] = v_tcoords[4]; 
        v_tcoords[11] = v_tcoords[5];
        
        glColor3f(1.0f, 1.0f, 1.0f);
	    glVertexPointer(3, GL_FLOAT, 0, &v_vert[0]);
	    glTexCoordPointer(2, GL_FLOAT, 0, &v_tcoords[0]);
	    glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}
