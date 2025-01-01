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
        model format container
        */
        bool Upload(unsigned int, sModelFormat);
        
        /*
        Parameter Info:
            position X offset
            position Y offset
            RGB color
        */
        void Render(int, int, sColor) const;

    private:
        
        void Destroy() const;

        int _num_indices;
        unsigned int _prog_id;
        unsigned int _vao;
        unsigned int _vbo;
        unsigned int _ebo;

    };

}
