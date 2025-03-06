#include "Text.h"

namespace Realgar
{
    Text::Text()
    {
        FT_Library ft;
        if (FT_Init_FreeType(&ft))
        {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        }

        FT_Face face;
        if (FT_New_Face(ft, "Game/Resources/Fonts/Testi.ttf", 0, &face))
        {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        }

        FT_Set_Pixel_Sizes(face, 0, 48);  
        if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;  
        }

        for (unsigned char c = 0; c < 128; c++)
        {
            // load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
        
            // generate texture
            Texture texture(face->glyph->bitmap.buffer, 1, face->glyph->bitmap.width, face->glyph->bitmap.rows);
            // now store character for later use
            Character character = {
                texture, 
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
        
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }
    
    Text::~Text()
    {
    }
}