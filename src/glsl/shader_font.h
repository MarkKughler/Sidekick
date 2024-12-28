#pragma once

namespace glsl
{

    class cShader_font
    {

    public:
        cShader_font();
        cShader_font(cShader_font&) = delete;
        ~cShader_font() { Destroy(); }

        bool Create();
        
        bool SetParameters(float[16]) const;

        unsigned int prog_id;
    
    private:
        
        void Destroy() const;
        
        unsigned int _vs_id;
        unsigned int _fs_id;

    };

}
