#pragma once

namespace glsl
{

    class cShader_font
    {

    public:
        cShader_font();
        cShader_font(cShader_font&) = delete;
        ~cShader_font() { /*empty destructor*/ }

        bool Create();
        void Destroy();

        bool SetParameters(float[16]);

        unsigned int prog_id;
    
    private:
        unsigned int _vs_id;
        unsigned int _fs_id;
        

    };

}
