#pragma once

#include "../core/model.h"

namespace gui
{

    class cMenubar
    {
    public:

        cMenubar();
        ~cMenubar() { Destroy(); }
        
        /*
        Parameter Info:
            monitor width
        */
        bool Create(int);
        
        
    private:

        void Destroy()
        { }

        core::cModel background;

    };

}
