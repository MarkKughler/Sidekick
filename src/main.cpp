
#include "resource/targetver.h"
#include "nublog.h"
#include "types.h"
#include "core/display.h"
#include "core/model.h"
#include "core/font.h"
#include "core/texture.h"
#include "glsl/shader_gui.h"


// global variables
constexpr auto max_load_string = 128;
char szTitle[max_load_string];             // title bar text
char szWindowClass[max_load_string];       // main window class name

HWND hwnd_parent;
RECT rect_screen_workarea;                 // screen space (x, y, w, h)
RECT rect_client_workarea;                 // client space (0, 0, w, h)


glsl::cShader_gui shader_gui;

core::cFont font_ui;
core::cFont font_ui_bold;

core::cModel model;

core::cTexture tex_gui;


struct Config
{
    sDims screen  = {1024, 576};
    sDims monitor = { 0, 0 };
    sMatrix ortho = { 0 };

} config;


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

    tex_gui.Load("data/textures/gui.tga");

    shader_gui.Create();

    font_ui.Initialize(shader_gui.prog_id, "data/fonts/Amble.ttf", 48);
    font_ui_bold.Initialize(shader_gui.prog_id, "data/fonts/Amble-Bold.ttf", 48);
   


    //                      x       y      u          v
    float w = 200.0f;
    float vdata_9sq[144] = {  0.0f,   8.0f, 0.0f,      0.015625f, // 0=>0.015625 0.015625=>0.03125 0.03125=>0.046875
                              8.0f,   8.0f, 0.015625f, 0.015625f,
                              0.0f,   0.0f, 0.0f,      0.0f,
                              8.0f,   0.0f, 0.015625f, 0.0f,
                              8.0f,   8.0f, 0.015625f, 0.012625f,
                             16.0f+w, 8.0f, 0.03125f,  0.012625f,
                              8.0f,   0.0f, 0.015625f, 0.0f,
                             16.0f+w, 0.0f, 0.03125f,  0.0f,
                             16.0f+w, 8.0f, 0.03125f,  0.012625f,
                             24.0f+w, 8.0f, 0.046875f, 0.012625f,
                             16.0f+w, 0.0f, 0.03125f,  0.0f,
                             24.0f+w, 0.0f, 0.046875f, 0.0f,

                              0.0f,   16.0f, 0.0f,      0.03125f, // 0=>0.015625 0.015625=>0.03125 0.03125=>0.046875
                              8.0f,   16.0f, 0.015625f, 0.03125f,
                              0.0f,    8.0f, 0.0f,      0.015625f,
                              8.0f,    8.0f, 0.015625f, 0.015625f,
                              8.0f,   16.0f, 0.015625f, 0.03125f,
                             16.0f+w, 16.0f, 0.03125f,  0.03125f,
                              8.0f,    8.0f, 0.015625f, 0.015625f,
                             16.0f+w,  8.0f, 0.03125f,  0.015625f,
                             16.0f+w, 16.0f, 0.03125f,  0.03125f,
                             24.0f+w, 16.0f, 0.046875f, 0.03125f,
                             16.0f+w,  8.0f, 0.03125f,  0.015625f,
                             24.0f+w,  8.0f, 0.046875f, 0.015625f,

                              0.0f,   24.0f, 0.0f,      0.046875f, // 0=>0.015625 0.015625=>0.03125 0.03125=>0.046875
                              8.0f,   24.0f, 0.015625f, 0.046875f,
                              0.0f,   16.0f, 0.0f,      0.03125f,
                              8.0f,   16.0f, 0.015625f, 0.03125f,
                              8.0f,   24.0f, 0.015625f, 0.046875f,
                             16.0f+w, 24.0f, 0.03125f,  0.046875f,
                              8.0f,   16.0f, 0.015625f, 0.03125f,
                             16.0f+w, 16.0f, 0.03125f,  0.03125f,
                             16.0f+w, 24.0f, 0.03125f,  0.046875f,
                             24.0f+w, 24.0f, 0.046875f, 0.046875f,
                             16.0f+w, 16.0f, 0.03125f,  0.03125f,
                             24.0f+w, 16.0f, 0.046875f, 0.03125f                         
    };
    unsigned int idata_9sq[54] = { 0,  1,  2,  2,  1,  3,   4,  5,  6,  6,  5,  7,   8,  9, 10, 10,  9, 11,
                                  12, 13, 14, 14, 13, 15,  16, 17, 18, 18, 17, 19,  20, 21, 22, 22, 21, 23,
                                  24, 25, 26, 26, 25, 27,  28, 29, 30, 30, 29, 31,  32, 33, 34, 34, 33, 35 };
    model.Upload(36, 54, 4, vdata_9sq, idata_9sq);                // todo: form binary data to this format

    
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
        if (msg.message == WM_QUIT) break;
        

        display.ogl.Begin();

        glDisable(GL_DEPTH_TEST);
        glUseProgram(shader_gui.prog_id);
        glUniformMatrix4fv(glGetUniformLocation(shader_gui.prog_id, "projection"), 1, false, config.ortho.mtx);
        glUniform3f(glGetUniformLocation(shader_gui.prog_id, "color"), 0.0f, 0.5f, 1.0f);
        tex_gui.Bind();
        model.Render();
        
        sColor color = { 0.9f, 0.9f, 0.9f };
        //glUseProgram(shader_font.prog_id);
        //glUniformMatrix4fv(glGetUniformLocation(shader_font.prog_id, "projection"), 1, false, config.ortho.mtx);
        font_ui.RenderText("Sidekick v0.1.0 - 2024", 10, 25, 0.29f, color);
        font_ui_bold.RenderText("Sidekick v0.1.0 - 2024", 10, 40, 0.29f, color);
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

