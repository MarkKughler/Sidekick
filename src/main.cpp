
#include "resource/targetver.h"
#include "nublog.h"
#include "types.h"
#include "core/display.h"
#include "core/model.h"
#include "core/font.h"
#include "glsl/shader_gui.h"
#include "glsl/shader_font.h"


// global variables
constexpr auto max_load_string = 128;
char szTitle[max_load_string];             // title bar text
char szWindowClass[max_load_string];       // main window class name

HWND hwnd_parent;
RECT rect_screen_workarea;                 // screen space (x, y, w, h)
RECT rect_client_workarea;                 // client space (0, 0, w, h)


glsl::cShader_gui shader_gui;
glsl::cShader_font shader_font;

core::cFont font;
core::cModel model;

struct Config
{
    sDims screen  = {1024, 768};
    sDims monitor = {   0,   0};
} config;


#ifdef NDEBUG
#pragma comment(lib, "../src/lib/freetype-2.13.3/freetype.lib")
int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prev_instance, _In_ LPSTR cmd_line, _In_ int cmd_show) {
#else
#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#pragma comment(lib, "../src/lib/freetype-2.13.3/freetype_d.lib")
int main(char* argc, int argv) {
    HINSTANCE instance = GetModuleHandle(0);
#endif

    LoadString(instance, IDS_APP_TITLE, szTitle, max_load_string);
    LoadString(instance, IDC_SIDEKICK_CLASS, szWindowClass, max_load_string);
    HACCEL hAccelTable = LoadAccelerators(instance, MAKEINTRESOURCE(IDC_SIDEKICK_ACC));

    core::cDisplay display;                             //
    config.monitor.x = GetSystemMetrics(SM_CXSCREEN);   // todo: work out these dimensions for accurate use cases
    config.monitor.y = GetSystemMetrics(SM_CYSCREEN);   //
    rect_screen_workarea = { -SM_CXBORDER * 2 + 1, 1,  GetSystemMetrics(SM_CXMAXIMIZED) + 1, GetSystemMetrics(SM_CYMAXIMIZED) - int(SM_CYBORDER * 2) + 2 };
    display.Create(szTitle, szWindowClass, config.screen.x, config.screen.y);
    display.ogl.SetState();
    
    shader_gui.Create();
    shader_font.Create();

    //                      x,     y,    z,    r,    g,    b
    float vdata[18] = { -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
                         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
                         0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f };
    unsigned int idata[3] = { 0, 1, 2 };
    model.Upload(3, 3, 6, vdata, idata);                // todo: form binary data to this format

    font.Initialize(shader_font.prog_id);
    

    MSG msg = { 0 };
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        display.ogl.Begin();
        float world[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
        shader_gui.SetParameters(world);
        model.Render();

        glDisable(GL_DEPTH_TEST);
        float text_proj[16] = { 1.0f/(config.screen.x/2),0,0,-1,  0,-1.0f/(config.screen.y/2),0,1, 0,0,-1,0, 0,0,0,1 };
        sColor color = { 0.9f, 0.9f, 0.9f };
        glUseProgram(shader_font.prog_id);
        glUniformMatrix4fv(glGetUniformLocation(shader_font.prog_id, "projection"), 1, false, text_proj);
        font.RenderText("Sidekick v0.1.0 - 2024", 25.0f, 25.0f, 0.52f, color);
        glEnable(GL_DEPTH_TEST);
        
        display.ogl.End();
    }

 
    
    model.Destroy();
    shader_gui.Destroy();
    shader_font.Destroy();
    
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

