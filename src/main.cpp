
#include "resource/targetver.h"
#include "nublog.h"
#include "types.h"

#include "glsl/shader_gui.h"

#include "core/display.h"
#include "core/font.h"
#include "core/model.h"
#include "core/texture.h"

#include "gui/builder.h"


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
core::cModel frame_menu;

// -------------------------------- textures ------------------------------------------
core::cTexture tex_gui_atlas; 


struct Config                                                     
{
    sDims screen  = { 1024, 576 };
    sDims monitor = { 0, 0 };
    sMatrix ortho = { 0 };

} config;

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

    core::cDisplay display;                             //
    config.monitor.x = GetSystemMetrics(SM_CXSCREEN);   // todo: work out these dimensions for accurate use cases
    config.monitor.y = GetSystemMetrics(SM_CYSCREEN);   //
    rect_screen_workarea = { -SM_CXBORDER * 2 + 1, 1,  GetSystemMetrics(SM_CXMAXIMIZED) + 1, GetSystemMetrics(SM_CYMAXIMIZED) - int(SM_CYBORDER * 2) + 2 };
    display.Create(szTitle, szWindowClass, config.monitor.x, config.monitor.y);
    display.ogl.SetState();
    display.ogl.GetOrtho(config.ortho);
    
    shader_gui.Create();
    
    font_ui.Initialize(shader_gui.prog_id, "data/fonts/Amble.ttf", 48);
    font_ui_bold.Initialize(shader_gui.prog_id, "data/fonts/Amble-Bold.ttf", 48);

    gui::sBuilder builder;
    sModelFormat f01, f02, fm;
    frame01.Upload(shader_gui.prog_id, builder.NineSquare(f01, 226.0f, 0.0f));
    frame02.Upload(shader_gui.prog_id, builder.NineSquare(f02, 226.0f, 300.0f));
    frame_menu.Upload(shader_gui.prog_id, builder.GradSquare(fm, (float)config.monitor.x, 0.0f));
    tex_gui_atlas.Load("data/textures/gui.tga");


    sColor white   = { 0.9f,  0.9f,  0.9f };
    sColor drkGray = { 0.09f, 0.11f, 0.2f };
    sColor red     = { 1.0f,  0.0f,  0.0f };
    sColor green   = { 0.0f,  1.0f,  0.0f };
    sColor blue    = { 0.0f,  0.5f,  1.0f };
    MSG msg = { 0 };
    while(1)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        if (msg.message == WM_QUIT) break;;
            
        display.ogl.Begin();
        glDisable(GL_DEPTH_TEST);

        glUseProgram(shader_gui.prog_id);
        glUniformMatrix4fv(shader_gui.loc_projection, 1, false, config.ortho.mtx);
        tex_gui_atlas.Bind();
        
        glUniform2f(shader_gui.loc_translation, 0.0f, 0.0f);
        frame_menu.Render(0.0f, 0.0f, drkGray);
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

