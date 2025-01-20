#pragma once

namespace glsl
{

    class cShader_vert_color
    {

    public:
        cShader_vert_color();
        cShader_vert_color(cShader_vert_color&) = delete;
        ~cShader_vert_color();

        bool Create();

        unsigned int prog_id;
        unsigned int loc_projection;  // float[16]
        unsigned int loc_offset;      // float[2]

    private:

        unsigned int _vs_id;
        unsigned int _fs_id;

    };

}
