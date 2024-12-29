#include "../types.h"
#include "../nublog.h"
#include "font.h"

core::cFont::cFont()
{
    _prog_id = 0;
    _vao = 0;
    _vbo = 0;
}


bool core::cFont::Initialize(unsigned int shader_id, const char* filename, int font_size)
{
    _prog_id = shader_id;
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        LOG_ERROR("cFont::Initialize", "Could not initialize freetype library")
        return false;
    }
    FT_Face face;
    if (FT_New_Face(ft, filename, 0, &face))
    {
        LOG_ERROR("cFont::Initialize", "Failed to load freetype font")
        return false;
    }
    FT_Set_Pixel_Sizes(face, 0, font_size);
    unsigned int font_texture_id;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (unsigned char item = 0; item < 128; item++)
    {
        if (FT_Load_Char(face, item, FT_LOAD_RENDER))
        {
            LOG_WARN("cFont::Initialize", "Failed to load glyph face")
            continue;
        }

        glGenTextures(1, &font_texture_id);
        glBindTexture(GL_TEXTURE_2D, font_texture_id);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RED,
            face->glyph->bitmap.width, face->glyph->bitmap.rows,
            0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        core::Glyph glyph = { font_texture_id,
            {(int)face->glyph->bitmap.width, (int)face->glyph->bitmap.rows},
            {(int)face->glyph->bitmap_left, (int)face->glyph->bitmap_top},
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        _glyphs.insert(std::pair<char, core::Glyph>(item, glyph));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
}


void core::cFont::RenderText(std::string text, int x, int y, float scale, sColor color)
{	
    glUniform3f(glGetUniformLocation(_prog_id, "color"), color.r, color.g, color.b);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(_vao);

    std::string::const_iterator iter;
    for (iter = text.begin(); iter != text.end(); iter++)
    {
        core::Glyph ch = _glyphs[*iter];

        float xpos = x + ch.bearing.x * scale;
        float ypos = y + (this->_glyphs['T'].bearing.y - ch.bearing.y) * scale;

        float w = ch.dims.x * scale;
        float h = ch.dims.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 0.0f },
            { xpos,     ypos,       0.0f, 0.0f },

            { xpos,     ypos + h,   0.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 0.0f },
        };
        
        glBindTexture(GL_TEXTURE_2D, ch.texture_id);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void core::cFont::Destroy() const
{
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &_vao);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &_vbo);
}