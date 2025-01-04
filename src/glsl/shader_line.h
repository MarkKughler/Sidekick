#pragma once

namespace glsl
{

    class cShader_line
    {

    public:

        cShader_line();
        cShader_line(cShader_line&) = delete;
        ~cShader_line();

        bool Create();

        unsigned int prog_id;
        unsigned int loc_projection;
        unsigned int loc_offset;
        unsigned int loc_color;

    private:

        unsigned int _vs_id;
        unsigned int _fs_id;

    };

}
