
#include "resource/targetver.h"
#include "nublog.h"
#include "types.h"

#include "glsl/shader_gui.h"
#include "glsl/shader_line.h"
#include "glsl/shader_grid2D.h"
#include "glsl/shader_vert_color.h"

#include "core/display.h"
#include "core/font.h"
#include "core/model.h"
#include "core/line.h"
#include "core/texture.h"

#include "gui/builder.h"
#include "gui/menubar.h"
#include "gui/grid_2D.h"
#include "gui/spline.h"
#include "gui/node.h"

#include <set>


// --------------------------- global variables ---------------------------------------
constexpr auto max_load_string = 128;
char szTitle[max_load_string];             // title bar text
char szWindowClass[max_load_string];       // main window class name
HWND hwnd_parent;
RECT rect_screen_workarea;                 // screen space (x, y, w, h)
RECT rect_client_workarea;                 // client space (0, 0, w, h)
sPoint pt_screen_offset = { 0.0f, 0.0f };  // virtual screen drag offset

core::cFont font_ui;
core::cFont font_ui_bold;

// -------------------------------- shaders ------------------------------------------- 
glsl::cShader_gui shader_gui;
glsl::cShader_line shader_line;
glsl::cShader_vert_color shader_vert_color;
glsl::cShader_grid2D shader_grid2D;

// --------------------------------- models -------------------------------------------

gui::cNodeContainer nodes;

core::cModel frame01;                    
core::cModel frame02;
core::cModel frame03;
core::cModel frame04;
core::cModel frame05;

// -------------------------------- textures ------------------------------------------
core::cTexture tex_gui_atlas; 


// ---------------------------------- gui ---------------------------------------------
gui::cMenubar menubar;



//    _____                .__  .__               __  .__                ___________       __                 
//   /  _  \ ______ ______ |  | |__| ____ _____ _/  |_|__| ____   ____   \_   _____/ _____/  |________ ___.__.
//  /  /_\  \\____ \\____ \|  | |  |/ ___\\__  \\   __\  |/  _ \ /    \   |    __)_ /    \   __\_  __ <   |  |
// /    |    \  |_> >  |_> >  |_|  \  \___ / __ \|  | |  (  <_> )   |  \  |        \   |  \  |  |  | \/\___  |
// \____|__  /   __/|   __/|____/__|\___  >____  /__| |__|\____/|___|  / /_______  /___|  /__|  |__|   / ____|
//         \/|__|   |__|                \/     \/                    \/          \/     \/             \/     
   
#ifdef NDEBUG
#pragma comment(lib, "../src/lib/freetype-2.13.3/freetype.lib")
int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prev_instance, _In_ LPSTR cmd_line, _In_ int cmd_show) {
#else
#include <crtdbg.h>
#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#pragma comment(lib, "../src/lib/freetype-2.13.3/freetype_d.lib")
int main(int argc, char* argv[]) {
    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
    //_CrtSetBreakAlloc(162);
    HINSTANCE instance = GetModuleHandle(0);
#endif

    LoadString(instance, IDS_APP_TITLE, szTitle, max_load_string);
    LoadString(instance, IDC_SIDEKICK_CLASS, szWindowClass, max_load_string);
    HACCEL hAccelTable = LoadAccelerators(instance, MAKEINTRESOURCE(IDC_SIDEKICK_ACC));

    sConfiguration config = { 0 };
    config.screen = { 1024, 768 };
    config.is_maxamized = false;
    config.monitor.x = GetSystemMetrics(SM_CXSCREEN);
    config.monitor.y = GetSystemMetrics(SM_CYSCREEN);

    core::cDisplay display;
    display.Create(szTitle, szWindowClass, &config);
    display.ogl.SetState();
    display.ogl.GetOrtho(config.ortho);
    
    shader_gui.Create();
    shader_line.Create();
    shader_vert_color.Create();
    shader_grid2D.Create();
    
    font_ui.Initialize(shader_gui.prog_id, "data/fonts/Amble.ttf", 48);
    font_ui_bold.Initialize(shader_gui.prog_id, "data/fonts/Amble-Bold.ttf", 48);

    gui::cGrid_2D grid2D({(float)config.monitor.x, (float)config.monitor.y});

    //gui::cNode node({ 0, 0 });


    gui::sBuilder builder;
    
    builder.NodeFrame(frame01.data, 2, 226.0f);
    builder.NineSquare(frame02.data, 226.0f, 300.0f);
    builder.NineSquare(frame03.data, 150.0f, 0.0f);
    builder.NineSquare(frame04.data, 150.0f, 0.0f);
    builder.NineSquare(frame05.data, 150.0f, 60.0f);
    frame01.Upload(shader_gui.prog_id);
    frame02.Upload(shader_gui.prog_id);
    frame03.Upload(shader_gui.prog_id);
    frame04.Upload(shader_gui.prog_id);
    frame05.Upload(shader_gui.prog_id);
    
   
    menubar.Create(&shader_gui, &font_ui, &config);

    /*core::cLine line;
    line.data.vdata = { {350, 211}, {371, 211} };
    line.Upload();
    line.PushVertex({ 530, 310 });
    line.PushVertex({ 549, 310 });*/
    
    gui::cSpline spline({ 350, 320 }, { 549, 420 });
    
    //gui::cLink link1({350, 211}, {371, 211}, {530, 310}, {549, 310});
    gui::cLink link3({350, 350}, {381, 500}, {518, 500}, {549, 460});
    gui::cLink link4({350, 380}, {371, 510}, {528, 510}, {549, 490});
    gui::cLinkContainer link_container;
    link_container.Initialize(10);
    //link_container.Push(link1);
    link_container.Push(link3);
    link_container.Push(link4);
    

    tex_gui_atlas.Load("data/textures/gui.tga");

    sRGB white   = { 0.9f,  0.9f,  0.9f };
    sRGB drkGray = { 0.09f, 0.11f, 0.2f };
    sRGB red     = { 1.0f,  0.0f,  0.0f };
    sRGB green   = { 0.0f,  1.0f,  0.0f };
    sRGB blue    = { 0.0f,  0.5f,  1.0f };


    POINT last_cursor_screen_pos = { 0 };
    MSG msg = { 0 };
    while(1)
    {
        // main message pump ---------------------------------------------------
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (msg.message == WM_QUIT) break;

        // mouse cursor information --------------------------------------------
        POINT cursor_screen_pos = { 0 };
        GetCursorPos(&cursor_screen_pos);
        RECT  screen_pos = { 0 };
        GetWindowRect(display.hWnd, &screen_pos);
        sPoint cursor_client_pos = { static_cast<float>(cursor_screen_pos.x) - screen_pos.left, static_cast<float>(cursor_screen_pos.y) - screen_pos.top };

        // ________                        ___________                             
        // \_____  \____________ __  _  __ \_   _____/___________    _____   ____  
        //  | |  \  \_  __ \__  \\ \/ \/ /  |    __) \_  __ \__  \  /     \_/ __ \ 
        //  | '___'  \  | \// __ \\     /   |     \   |  | \// __ \|  Y Y  \  ___/ 
        // /_______  /__|  (____  /\/\_/    \___  /   |__|  (____  /__|_|  /\___  >
        //         \/           \/              \/               \/      \/     \/ 
        
        display.ogl.Begin();
        glDisable(GL_DEPTH_TEST);
        

        glUseProgram(shader_grid2D.prog_id);
        glUniformMatrix4fv(shader_grid2D.loc_projection, 1, false, config.ortho.mtx);
        glUniform2f(shader_grid2D.loc_screenRes, config.screen.x, config.screen.y);
        glUniform2f(shader_grid2D.loc_offset, pt_screen_offset.x, pt_screen_offset.y);
        grid2D.Render();

        glUseProgram(shader_gui.prog_id);
        glUniformMatrix4fv(shader_gui.loc_projection, 1, false, config.ortho.mtx);
        glUniform2f(shader_gui.loc_translation, 0.0f, 0.0f);
        tex_gui_atlas.Bind();
        
        
        int event = menubar.Draw(cursor_client_pos.x, cursor_client_pos.y, display.lButtonDown);
        if (display.window_dragging) event = 4;
        switch (event)
        {
        case 1: PostQuitMessage(0); break; // todo: save working doc
        case 2: if (config.is_maxamized) ShowWindow(display.hWnd, SW_SHOWNORMAL);
                else ShowWindow(display.hWnd, SW_SHOWMAXIMIZED);
                    
                config.is_maxamized = !config.is_maxamized;
                display.ogl.GetOrtho(config.ortho);
                display.lButtonDown = false;
                break;
        case 3: ShowWindow(display.hWnd, SW_SHOWMINIMIZED); 
                display.lButtonDown = false;
                break;
        case 4:
                {
                display.window_dragging = true;
                int sp_x = screen_pos.left + (cursor_screen_pos.x - last_cursor_screen_pos.x);
                int sp_y = screen_pos.top + (cursor_screen_pos.y - last_cursor_screen_pos.y);
                if (sp_x < 0) sp_x = 0;
                if (sp_y < 0) sp_y = 0;
                if (sp_x + config.screen.x > config.monitor.x) sp_x = config.monitor.x - config.screen.x;
                if (sp_y + config.screen.y > config.monitor.y) sp_y = config.monitor.y - config.screen.y;
                MoveWindow(display.hWnd, sp_x, sp_y, config.screen.x, config.screen.y, FALSE);
                break;
                }
        }
        
        // handle virtual screen manipulation
        if (display.input_state[VK_SHIFT] && display.lButtonDown)
        {
            pt_screen_offset.x += (cursor_screen_pos.x - last_cursor_screen_pos.x);
            pt_screen_offset.y += (cursor_screen_pos.y - last_cursor_screen_pos.y);
        }

        tex_gui_atlas.Bind();
        glUniform2f(shader_gui.loc_translation, 100.0f, 200.0f); // group translation
        frame01.Render(0.f, 0.f , blue);
        frame02.Render(0.f, 100.f, green);
        frame03.Render(449.0f, 99.0f, blue);
        frame04.Render(449.0f, 208.0f, green);
        frame05.Render(449.0f, 248.0f, green);
      
        glUniform2f(shader_gui.loc_offset, 40.0f, 7.0f); // glyph baseline offset
        //font_ui.RenderText("Sidekick v0.1.0 - 2025", 10, 30, 0.29f, white);
        font_ui_bold.RenderText("Sidekick v0.1.0 - 2025", 10, 0, 0.29f, white);

        //350, 211   549, 310
        sRect test_output = { 340, 201, 350, 221 };
        



        if (display.lButtonDown) 
        {
            // test nodes
            if (cursor_client_pos.x > test_output.x && cursor_client_pos.x < test_output.w && cursor_client_pos.y > test_output.y && cursor_client_pos.y < test_output.h)
            {
                link_container.BeginNewLink(cursor_client_pos);
            }
        }

        sRGB line_color = { 0.9f, 0.9f, 0.9f };
        glUseProgram(shader_line.prog_id);
        glUniformMatrix4fv(shader_line.loc_projection, 1, GL_FALSE, config.ortho.mtx);
        glUniform2f(shader_line.loc_offset, 0.f, 0.f);
        glUniform3f(shader_line.loc_color, line_color.r, line_color.g, line_color.b);
        //line.Render();
        spline.Render();



        glUseProgram(shader_vert_color.prog_id);
        glUniformMatrix4fv(shader_vert_color.loc_projection, 1, GL_FALSE, config.ortho.mtx);
        glUniform2f(shader_vert_color.loc_offset, 0.f, 0.f);
        link_container.Render(cursor_client_pos, display.lButtonDown);



        glEnable(GL_DEPTH_TEST);
        display.ogl.End();

        
 
        last_cursor_screen_pos = cursor_screen_pos;
    }

    return (int)msg.wParam;
}



// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// About Box Message handler - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
INT_PTR CALLBACK proc_About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

