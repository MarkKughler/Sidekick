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
        ~cModel() { Destroy(); }

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
        void Render(int, int, sRGB) const;

        sModelFormat data;

    private:
        
        void Destroy() const;

        unsigned int _prog_id;
        unsigned int _vao;
        unsigned int _vbo;
        unsigned int _ebo;

    };

}
