#include "menubar.h"
#include "../core/font.h"


gui::cMenubar::cMenubar()
{
    _font = nullptr;
    _pConfig = nullptr;
}


bool gui::cMenubar::Create(glsl::cShader_gui &shader, core::cFont &font, sConfiguration* config)
{
    _font = &font;
    _pConfig = config;
    
    sBuilder builder;
    builder.GradSquare(_background.data, (float)config->monitor.x, 0.0f);
    builder.Icon(_favicon.data, 0, 19);
    builder.Icon(_minimize.data, 3, 19);
    builder.Icon(_maximize.data, 2, 19);
    builder.Icon(_close.data, 1, 19);

    _background.Upload(shader.prog_id);
    _favicon.Upload(shader.prog_id);
    _minimize.Upload(shader.prog_id);
    _maximize.Upload(shader.prog_id);
    _close.Upload(shader.prog_id);
    return true;
}


int gui::cMenubar::Draw(int cur_pos_x, int cur_pos_y, bool lbutton_down)
{
    bool is_over_close = false;
    bool is_over_maximize = false;
    bool is_over_minimize = false;

    if (cur_pos_y < 24)
    {
        if (cur_pos_x > _pConfig->screen.x - 32 && cur_pos_x < _pConfig->screen.x ) is_over_close = true;
        if (cur_pos_x > _pConfig->screen.x - 64 && cur_pos_x < _pConfig->screen.x - 32) is_over_maximize = true;
        if (cur_pos_x > _pConfig->screen.x - 96 && cur_pos_x < _pConfig->screen.x - 64) is_over_minimize = true;        
    }

    sRGB rgb_default = { 0.8f, 0.8f, 0.8f };
    sRGB rgb_over    = { 0.0f, 0.5f, 1.0f };
    _background.Render(0.0f, 0.0f, { 0.09f, 0.11f, 0.2f });
    _favicon.Render(0.0f, 0.0f, { 0.8f, 0.8f, 0.8f });
    _minimize.Render(_pConfig->screen.x - 96.f, 0.0f, is_over_minimize ? rgb_over : rgb_default);
    _maximize.Render(_pConfig->screen.x - 64.f, 0.0f, is_over_maximize ? rgb_over : rgb_default);
    _close.Render(_pConfig->screen.x - 32.f, 0.0f, is_over_close ? rgb_over : rgb_default);

    int event = 0;
    if (is_over_close && lbutton_down)    event = 1;
    if (is_over_maximize && lbutton_down) event = 2; 
    if (is_over_minimize && lbutton_down) event = 3;
    return event;
}
