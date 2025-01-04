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
            shader program class
            font instance pointer
            configuration pointer
        */
        bool Create(glsl::cShader_gui* shader, core::cFont* font, sConfiguration* config);

        int Draw(int cur_pos_x, int cur_pos_y, bool lbutton_down);
          
    private:

        void Destroy()
        { }

        sConfiguration* _pConfig;
        core::cFont* _pFont;
        glsl::cShader_gui* _pShader;

        core::cModel _background;           // titlebar background
        core::cModel _favicon;              // icon favicon
        core::cModel _minimize;             // icon minimize
        core::cModel _maximize;             // icon maximize
        core::cModel _restore;              // icon restore
        core::cModel _close;                // icon close
        core::cModel _cfFile;               // menu item child frame background
        core::cModel _cfEdit;               // menu item child frame background
        core::cModel _cfMode;                // menu item child frame background
        
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
            //std::vector<sMenuItem> children;
            std::vector<std::string> children;

            void Init(std::string in_label, sDims in_pos)
            {
                label = in_label;
                pos = in_pos;
            }

            void AddChild(std::string in_label)
            {
                children.push_back(in_label);
            }
        };

        sMenuItem _miFile;                   // root menu item File
        sMenuItem _miEdit;                   // root menu item Edit
        sMenuItem _miMode;                   // root menu item Mode
        sMenuItem _separator;                // (positioning stop)

    };

}
