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
        ~cFont() { Destroy(); }

        /*
        Parameter Info:
        shader program id
        font filename
        font size
        */
        bool Initialize(unsigned int, const char*, int);

        
        
        /*
        Parameter Info:
        text string
        position X offset
        position Y offset
        percentage of font size
        foreground color
        */
        void RenderText(std::string, int, int, float, sRGB);

       

    private:
        
        void Destroy() const;
        
        std::map<GLchar, core::Glyph> _glyphs;
        unsigned int _prog_id;
        unsigned int _vao;
        unsigned int _vbo;

    };

}
