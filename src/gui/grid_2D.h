#pragma once
#include "../types.h"

namespace gui
{

    class cGrid_2D
    {
    public:
        cGrid_2D(sPoint pt);
        void Render();

    private:
        unsigned int _vao;
        unsigned int _vbo;        

    };

}
