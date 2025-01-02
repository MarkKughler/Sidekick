#pragma once

#include "../glsl/shader_gui.h"
#include "../core/model.h"
#include "../core/font.h"

namespace gui
{

    class cMenubar
    {
    public:

        cMenubar();
        ~cMenubar() { Destroy(); }
        
        /*
        Parameter Info:
            shader program id
            font
            configuration pounter
        */
        bool Create(glsl::cShader_gui &shader, core::cFont &font, sConfiguration* config);

        int Draw(int cur_pos_x, int cur_pos_y, bool lbutton_down);
          
        
    private:

        void Destroy()
        { }

        sConfiguration* _pConfig;
        core::cModel _background;
        core::cModel _favicon;
        core::cModel _minimize;
        core::cModel _maximize;
        core::cModel _close;
        core::cFont* _font;

    };

}
