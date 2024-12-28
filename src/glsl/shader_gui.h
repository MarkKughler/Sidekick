#pragma once

namespace glsl
{

    class cShader_gui
    {

    public:
        cShader_gui();
        cShader_gui(cShader_gui&) = delete;
        ~cShader_gui() { /*empty destructor*/ }

        bool Create();
        void Destroy();

        bool SetParameters(float[16]);

    private:
        unsigned int _vs_id;
        unsigned int _fs_id;
        unsigned int _prog_id;

    };

}