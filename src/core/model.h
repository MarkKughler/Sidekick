#pragma once
#include "../types.h"
#include "../gui/builder.h"

namespace core
{

    class cModel
    {

    public:

        cModel();
        cModel(cModel&) = delete;
        ~cModel();

        /*
        Parameter Info:
        shader program id
        */
        bool Upload(unsigned int);
        
        /*
        Parameter Info:
            position X offset
            position Y offset
            RGB color
        */
        void Render(float x, float y, sRGB) const;

        sModelFormat data;

    private:
        
        unsigned int _prog_id;
        unsigned int _vao;
        unsigned int _vbo;
        unsigned int _ebo;

    };

}
