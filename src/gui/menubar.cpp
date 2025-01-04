#include "menubar.h"
#include "../core/font.h"


gui::cMenubar::cMenubar()
{
    _pFont = nullptr;
    _pConfig = nullptr;
    _pShader = nullptr;
}


bool gui::cMenubar::Create(glsl::cShader_gui* shader, core::cFont* font, sConfiguration* config)
{
    _pShader = shader;
    _pFont = font;
    _pConfig = config;
    
    sBuilder builder;
    builder.GradSquare(_background.data, (float)config->monitor.x, 0.0f);
    builder.Icon(_favicon.data, 0, 19);
    builder.Icon(_minimize.data, 3, 19);
    builder.Icon(_maximize.data, 2, 19);
    builder.Icon(_restore.data, 4, 19);
    builder.Icon(_close.data, 1, 19);
    builder.NineSquare(_cfFile.data, 100, 24);
    builder.NineSquare(_cfEdit.data, 100, 48);
    builder.NineSquare(_cfMode.data, 160, 48);

    _background.Upload(shader->prog_id);
    _favicon.Upload(shader->prog_id);
    _minimize.Upload(shader->prog_id);
    _maximize.Upload(shader->prog_id);
    _restore.Upload(shader->prog_id);
    _close.Upload(shader->prog_id);
    _cfFile.Upload(shader->prog_id);
    _cfEdit.Upload(shader->prog_id);
    _cfMode.Upload(shader->prog_id);

    _miFile.Init("File", { 42, 0 });
    _miFile.AddChild("New");
    _miFile.AddChild("Save");
    _miEdit.Init("Edit", { 81, 0 });
    _miEdit.AddChild("Cut");
    _miEdit.AddChild("Copy");
    _miEdit.AddChild("Paste");
    _miMode.Init("Mode", { 120, 0 });
    _miMode.AddChild("Class Designer");
    _miMode.AddChild("PDF Writer");
    _miMode.AddChild("Documentation Writer");
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
    bool is_over_file_item[2] = { false, false };
    bool is_over_edit_item[3] = { false, false, false };
    bool is_over_mode_item[3] = { false, false, false };

    if (cur_pos_y < 24)
    {
        if (cur_pos_x > _pConfig->screen.x - 32 && cur_pos_x < _pConfig->screen.x) is_over_close = true;
        if (cur_pos_x > _pConfig->screen.x - 64 && cur_pos_x < _pConfig->screen.x - 32) is_over_maximize = true;
        if (cur_pos_x > _pConfig->screen.x - 96 && cur_pos_x < _pConfig->screen.x - 64) is_over_minimize = true;
        if (cur_pos_x > _miFile.pos.x && cur_pos_x < _miEdit.pos.x) is_over_file = true;
        if (cur_pos_x > _miEdit.pos.x && cur_pos_x < _miMode.pos.x) is_over_edit = true;
        if (cur_pos_x > _miMode.pos.x && cur_pos_x < _separator.pos.x) is_over_mode = true;
    }
    if (cur_pos_x > _miFile.pos.x && cur_pos_x < _miFile.pos.x + 100)
    {
        if (cur_pos_y > 24 && cur_pos_y < 48) is_over_file_item[0] = true;
        if (cur_pos_y > 48 && cur_pos_y < 72) is_over_file_item[1] = true;
    }
    if (cur_pos_x > _miEdit.pos.x && cur_pos_x < _miEdit.pos.x + 100)
    {
        if (cur_pos_y > 24 && cur_pos_y < 48) is_over_edit_item[0] = true;
        if (cur_pos_y > 48 && cur_pos_y < 72) is_over_edit_item[1] = true;
        if (cur_pos_y > 72 && cur_pos_y < 96) is_over_edit_item[2] = true;
    }
    if (cur_pos_x > _miMode.pos.x && cur_pos_x < _miMode.pos.x + 160)
    {
        if (cur_pos_y > 24 && cur_pos_y < 48) is_over_mode_item[0] = true;
        if (cur_pos_y > 48 && cur_pos_y < 72) is_over_mode_item[1] = true;
        if (cur_pos_y > 72 && cur_pos_y < 96) is_over_mode_item[2] = true;
    }

    // menu bar graphics -------------------------------------------------------
    sRGB rgb_default = { 0.7f, 0.7f, 0.7f };
    sRGB rgb_over    = { 0.0f, 0.5f, 1.0f };
    sRGB rgb_frame   = { 0.0f, 0.2f, 0.7f };
    _background.Render(0.0f, 0.0f, { 0.09f, 0.11f, 0.2f });
    _favicon.Render(0.0f, 0.0f, { 0.8f, 0.8f, 0.8f });
    _minimize.Render(_pConfig->screen.x - 96.f, 0.0f, is_over_minimize ? rgb_over : rgb_default);
    if (_pConfig->is_maxamized)
        _restore.Render(_pConfig->screen.x - 64.f, 0.0f, is_over_maximize ? rgb_over : rgb_default);
    else
        _maximize.Render(_pConfig->screen.x - 64.f, 0.0f, is_over_maximize ? rgb_over : rgb_default);
    _close.Render(_pConfig->screen.x - 32.f, 0.0f, is_over_close ? rgb_over : rgb_default);

    if (_miFile.show_child) _cfFile.Render((float)_miFile.pos.x, 26.f, rgb_frame);
    if (_miEdit.show_child) _cfEdit.Render((float)_miEdit.pos.x, 26.f, rgb_frame);
    if (_miMode.show_child) _cfMode.Render((float)_miMode.pos.x, 26.f, rgb_frame);

    // menu bar text -----------------------------------------------------------
    glUniform2f(_pShader->loc_offset, 0.f, 8.f); // font base line vertical offset
    _pFont->RenderText(_miFile.label, _miFile.pos.x, 0, 0.28f, is_over_file ? rgb_over : rgb_default);
    if (_miFile.show_child)
        for (int i = 0; i < _miFile.children.size(); ++i)
            _pFont->RenderText(_miFile.children[i], _miFile.pos.x + 32, (i + 1) * 24, 0.28f, is_over_file_item[i] ? rgb_over : rgb_default);
   
    _pFont->RenderText(_miEdit.label, _miEdit.pos.x, 0, 0.28f, is_over_edit ? rgb_over : rgb_default);
    if (_miEdit.show_child)
        for (int i = 0; i < _miEdit.children.size(); ++i)
            _pFont->RenderText(_miEdit.children[i], _miEdit.pos.x + 32, (i + 1) * 24, 0.28f, is_over_edit_item[i] ? rgb_over : rgb_default);

    _pFont->RenderText(_miMode.label, _miMode.pos.x, 0, 0.28f, is_over_mode ? rgb_over : rgb_default);
    if (_miMode.show_child)
        for (int i = 0; i < _miMode.children.size(); ++i)
            _pFont->RenderText(_miMode.children[i], _miMode.pos.x + 32, (i + 1) * 24, 0.28f, is_over_mode_item[i] ? rgb_over : rgb_default);

    // event dispatch ----------------------------------------------------------
    if (lbutton_down)
    {   
        _miFile.show_child = false;
        _miEdit.show_child = false;
        _miMode.show_child = false;
        if (is_over_close)    return 1;                                              // window close
        if (is_over_maximize) return 2;                                              // window maximize
        if (is_over_minimize) return 3;                                              // window minimize
        
        if (is_over_file) { _miFile.show_child = true; return 0; }                   // root menu item file
        if (is_over_file_item[0]) { _miFile.show_child = false; return 100; }
        if (is_over_file_item[1]) { _miFile.show_child = false; return 101; }
        if (is_over_edit) { _miEdit.show_child = true; return 0; }                   // root menu item edit
        if (is_over_edit_item[0]) { _miEdit.show_child = false; return 200; }
        if (is_over_edit_item[1]) { _miEdit.show_child = false; return 201; }
        if (is_over_edit_item[2]) { _miEdit.show_child = false; return 202; }
        if (is_over_mode) { _miMode.show_child = true; return 0; }                   // root menu item mode
        if (is_over_mode_item[0]) { _miMode.show_child = false; return 300; }
        if (is_over_mode_item[1]) { _miMode.show_child = false; return 301; }
        if (is_over_mode_item[2]) { _miMode.show_child = false; return 302; }

        else if (cur_pos_y < 24 && !_pConfig->is_maxamized) return 4;                // window move
    }
    return 0;
}
