
#include "resource/targetver.h"
#include "nublog.h"
#include "types.h"

#include "glsl/shader_gui.h"

#include "core/display.h"
#include "core/font.h"
#include "core/model.h"
#include "core/texture.h"

#include "gui/builder.h"
#include "gui/menubar.h"


// --------------------------- global variables ---------------------------------------
constexpr auto max_load_string = 128;
char szTitle[max_load_string];             // title bar text
char szWindowClass[max_load_string];       // main window class name
HWND hwnd_parent;
RECT rect_screen_workarea;                 // screen space (x, y, w, h)
RECT rect_client_workarea;                 // client space (0, 0, w, h)

core::cFont font_ui;
core::cFont font_ui_bold;

// -------------------------------- shaders ------------------------------------------- 
glsl::cShader_gui shader_gui;


// --------------------------------- models -------------------------------------------
core::cModel frame01;                    
core::cModel frame02;

// -------------------------------- textures ------------------------------------------
core::cTexture tex_gui_atlas; 

// ---------------------------------- gui ---------------------------------------------
gui::cMenubar menubar;


/*struct Config
{
    sDims screen  = { 1024, 768 };
    sDims monitor = { 0, 0 };
    sMatrix ortho = { 0 };

} config;*/

   /*   ****  ****  *    ***  ***   **  ***** ***  ***  *   *    ***** *   * ***** ****  *   *
  *  *  *   * *   * *     *  *     *  *   *    *  *   * **  *    *     **  *   *   *   * *   *
  ****  ****  ****  *     *  *     ****   *    *  *   * * * *    ***   * * *   *   ****   * *
 *    * *     *     *     *  *    *    *  *    *  *   * *  **    *     *  **   *   *  *    *
 *    * *     *     **** ***  *** *    *  *   ***  ***  *   *    ****  *   *   *   *   *   */  
#ifdef NDEBUG
#pragma comment(lib, "../src/lib/freetype-2.13.3/freetype.lib")
int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prev_instance, _In_ LPSTR cmd_line, _In_ int cmd_show) {
#else
#include <crtdbg.h>
#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#pragma comment(lib, "../src/lib/freetype-2.13.3/freetype_d.lib")
int main(char* argc, int argv) {
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
    
    font_ui.Initialize(shader_gui.prog_id, "data/fonts/Amble.ttf", 48);
    font_ui_bold.Initialize(shader_gui.prog_id, "data/fonts/Amble-Bold.ttf", 48);

    gui::sBuilder builder;
    builder.NineSquare(frame01.data, 226.0f, 0.0f);
    builder.NineSquare(frame02.data, 226.0f, 300.0f);
    frame01.Upload(shader_gui.prog_id);
    frame02.Upload(shader_gui.prog_id);
   
    menubar.Create(shader_gui, &font_ui, &config);


    tex_gui_atlas.Load("data/textures/gui.tga");

    sRGB white   = { 0.9f,  0.9f,  0.9f };
    sRGB drkGray = { 0.09f, 0.11f, 0.2f };
    sRGB red     = { 1.0f,  0.0f,  0.0f };
    sRGB green   = { 0.0f,  1.0f,  0.0f };
    sRGB blue    = { 0.0f,  0.5f,  1.0f };


    MSG msg = { 0 };
    while(1)
    {
        display.lButtonDown = false;

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        if (msg.message == WM_QUIT) break;

        POINT cursor_pos = { 0 };
        GetCursorPos(&cursor_pos);
            
        display.ogl.Begin();
        glDisable(GL_DEPTH_TEST);

        glUseProgram(shader_gui.prog_id);
        glUniformMatrix4fv(shader_gui.loc_projection, 1, false, config.ortho.mtx);
        glUniform2f(shader_gui.loc_translation, 0.0f, 0.0f);
        tex_gui_atlas.Bind();
        
        
        int event = menubar.Draw(cursor_pos.x, cursor_pos.y, display.lButtonDown);
        switch (event)
        {
        case 1: PostQuitMessage(0); break; // todo: save working doc
        case 2: if (config.is_maxamized) ShowWindow(display.hWnd, SW_SHOWNORMAL);
                else ShowWindow(display.hWnd, SW_SHOWMAXIMIZED);
                    
                config.is_maxamized = !config.is_maxamized;
                display.ogl.GetOrtho(config.ortho);
                break;
        case 3: ShowWindow(display.hWnd, SW_SHOWMINIMIZED); break;
        default: break;
        }
        
        tex_gui_atlas.Bind();
        glUniform2f(shader_gui.loc_translation, 100.0f, 200.0f); // group translation
        frame01.Render(0, 0, blue);
        frame02.Render(0, 100, green);
      
        glUniform2f(shader_gui.loc_offset, 0.0f, 7.0f); // glyph baseline offset
        font_ui.RenderText("Sidekick v0.1.0 - 2024", 10, 30, 0.29f, white);
        font_ui_bold.RenderText("Sidekick v0.1.0 - 2024", 10, 0, 0.29f, white);

        glEnable(GL_DEPTH_TEST);
        display.ogl.End();

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

