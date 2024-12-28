#pragma once

#include <string>
#include <map>
#include "glad/glad_4_6.h"
#include "freetype-2.13.3/include/ft2build.h"
#include FT_FREETYPE_H

namespace core
{

    struct Glyph
    {
        unsigned int texture_id;
        sDims dims;
        sDims bearing;
        unsigned int advance;
    };


    class cFont
    {

    public:

        cFont();
        cFont(cFont&) = delete;
        ~cFont() { /*empty destructor*/ }

        /*
        Parameter Info:
        shader program id
        */
        bool Initialize(unsigned int);

        /*
        Parameter Info:
        text string
        screen position X
        screen position Y
        percentage of font size (28px)
        foreground color
        */
        void RenderText(std::string, int, int, float, sColor);

    private:
        
        std::map<GLchar, core::Glyph> _glyphs;
        unsigned int _prog_id;
        unsigned int _vao;
        unsigned int _vbo;

    };

}
