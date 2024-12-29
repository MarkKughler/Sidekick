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
    
    private:
        
        void Destroy() const;
        
        unsigned int _vs_id;
        unsigned int _fs_id;

    };

}
