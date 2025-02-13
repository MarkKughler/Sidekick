#pragma once

namespace glsl
{

    class cShader_gui
    {

    public:
        cShader_gui();
        cShader_gui(cShader_gui&) = delete;
        ~cShader_gui() { Destroy(); }

        bool Create();
        
        bool SetParameters(float[16]) const;

        unsigned int prog_id;
        unsigned int loc_projection;  // float[16]
        unsigned int loc_translation; // float[2]
        unsigned int loc_offset;      // float[2]
        unsigned int loc_color;       // float[3]
        unsigned int loc_zoom;        // float
    
    private:
        
        void Destroy() const;
        
        unsigned int _vs_id;
        unsigned int _fs_id;

    };

}
