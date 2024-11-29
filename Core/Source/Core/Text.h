#pragma once

#include <glm/glm.hpp>
#include <map>

#include "Texture.h"

#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H  

namespace RED
{
    class Text
    {

    public:
        Text();
        ~Text();
    private:
        FT_Library ft;
    };
    
    struct Character {
        Texture TextureID;  // ID handle of the glyph texture
        glm::ivec2   Size;       // Size of glyph
        glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
        FT_Pos Advance;    // Offset to advance to next glyph
    };

    std::map<char, Character> Characters;
} 
