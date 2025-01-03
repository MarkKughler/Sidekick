#include "menubar.h"
#include "../core/font.h"


gui::cMenubar::cMenubar()
{
    _pFont = nullptr;
    _pConfig = nullptr;
}


bool gui::cMenubar::Create(glsl::cShader_gui &shader, core::cFont* font, sConfiguration* config)
{
    _pShader = &shader;
    _pFont = font;
    _pConfig = config;
    
    sBuilder builder;
    builder.GradSquare(_background.data, (float)config->monitor.x, 0.0f);
    builder.Icon(_favicon.data, 0, 19);
    builder.Icon(_minimize.data, 3, 19);
    builder.Icon(_maximize.data, 2, 19);
    builder.Icon(_restore.data, 4, 19);
    builder.Icon(_close.data, 1, 19);

    _background.Upload(shader.prog_id);
    _favicon.Upload(shader.prog_id);
    _minimize.Upload(shader.prog_id);
    _maximize.Upload(shader.prog_id);
    _restore.Upload(shader.prog_id);
    _close.Upload(shader.prog_id);

    _miFile.label = "File";
    _miFile.pos = { 42, 0 };
    _miEdit.label = "Edit";
    _miEdit.pos = { 81, 0 };
    _miMode.label = "Mode";
    _miMode.pos = { 120, 0 };
    _separator.pos = { 180, 0 };

    return true;
}


int gui::cMenubar::Draw(int cur_pos_x, int cur_pos_y, bool lbutton_down)
{
    bool is_over_close = false;
    bool is_over_maximize = false;
    bool is_over_minimize = false;
    bool is_over_file = false;
    bool is_over_edit = false;
    bool is_over_mode = false;

    if (cur_pos_y < 24)
    {
        if (cur_pos_x > _pConfig->screen.x - 32 && cur_pos_x < _pConfig->screen.x) is_over_close = true;
        if (cur_pos_x > _pConfig->screen.x - 64 && cur_pos_x < _pConfig->screen.x - 32) is_over_maximize = true;
        if (cur_pos_x > _pConfig->screen.x - 96 && cur_pos_x < _pConfig->screen.x - 64) is_over_minimize = true;
        if (cur_pos_x > _miFile.pos.x && cur_pos_x < _miEdit.pos.x) is_over_file = true;
        if (cur_pos_x > _miEdit.pos.x && cur_pos_x < _miMode.pos.x) is_over_edit = true;
        if (cur_pos_x > _miMode.pos.x && cur_pos_x < _separator.pos.x) is_over_mode = true;
    }

    sRGB rgb_default = { 0.7f, 0.7f, 0.7f };
    sRGB rgb_over    = { 0.0f, 0.5f, 1.0f };
    _background.Render(0.0f, 0.0f, { 0.09f, 0.11f, 0.2f });
    _favicon.Render(0.0f, 0.0f, { 0.8f, 0.8f, 0.8f });
    _minimize.Render(_pConfig->screen.x - 96.f, 0.0f, is_over_minimize ? rgb_over : rgb_default);
    if (_pConfig->is_maxamized)
        _restore.Render(_pConfig->screen.x - 64.f, 0.0f, is_over_maximize ? rgb_over : rgb_default);
    else
        _maximize.Render(_pConfig->screen.x - 64.f, 0.0f, is_over_maximize ? rgb_over : rgb_default);
    _close.Render(_pConfig->screen.x - 32.f, 0.0f, is_over_close ? rgb_over : rgb_default);


    glUniform2f(_pShader->loc_offset, 0.f, 8.f);
    _pFont->RenderText(_miFile.label, _miFile.pos.x, 0, 0.28f, is_over_file ? rgb_over : rgb_default);
    _pFont->RenderText(_miEdit.label, _miEdit.pos.x, 0, 0.28f, is_over_edit ? rgb_over : rgb_default);
    _pFont->RenderText(_miMode.label, _miMode.pos.x, 0, 0.28f, is_over_mode ? rgb_over : rgb_default);

    if (lbutton_down)
    {   // event dispatch
        if (is_over_close)    return 1;
        if (is_over_maximize) return 2;
        if (is_over_minimize) return 3;
        if (is_over_file)     return 4;
        if (is_over_edit)     return 5;
        if (is_over_mode)     return 6;
    }
    return 0;
}
