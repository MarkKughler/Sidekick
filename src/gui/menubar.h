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
        bool Create(glsl::cShader_gui &shader, core::cFont* font, sConfiguration* config);

        int Draw(int cur_pos_x, int cur_pos_y, bool lbutton_down);
          
    private:

        void Destroy()
        { }

        sConfiguration* _pConfig;
        core::cFont* _pFont;
        glsl::cShader_gui* _pShader;

        core::cModel _background;
        core::cModel _favicon;
        core::cModel _minimize;
        core::cModel _maximize;
        core::cModel _restore;
        core::cModel _close;
        
        struct sMenuItem
        {
            bool is_disabled   = false;     // display grayed
            bool has_icon      = false;     // displays an icon
            bool has_children  = false;     // displays an arrow marker
            bool show_child    = false;     // children are expanded
            sDims pos = { 0 };              // X: pixel horz position Y: child slot number
            core::cModel icon;
            std::string label  = "\0";
            char shortcut      = ' '; // ????
            std::vector<sMenuItem> children;
        };

        sMenuItem _miFile;
        sMenuItem _miEdit;
        sMenuItem _miMode;
        sMenuItem _separator;

    };

}
