#pragma once

namespace glsl
{

    class cShader_grid2D
    {

    public:

        cShader_grid2D();
        cShader_grid2D(cShader_grid2D&) = delete;
        ~cShader_grid2D();

        bool Create();

        unsigned int prog_id;
        unsigned int loc_projection;
        unsigned int loc_screenRes;
        unsigned int loc_offset;
        unsigned int loc_zoom;

    private:

        unsigned int _vs_id;
        unsigned int _fs_id;

    };

}
