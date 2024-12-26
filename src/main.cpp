
#include "resource/targetver.h"
#include "core/display.h"
#include "core/shader_gui.h"
#include "core/model.h"
#include <string>
#include <vector>

// global variables
#define MAX_LOADSTRING 128
char szTitle[MAX_LOADSTRING];              // title bar text
char szWindowClass[MAX_LOADSTRING];        // main window class name

HWND hwnd_parent;
RECT rect_screen_workarea;                 // screen space (x, y, w, h)
RECT rect_client_workarea;                 // client space (0, 0, w, h)


core::cShader_gui shader_gui;
core::cModel model;

struct Config
{
    std::vector<int> screen  = { 1024, 768 };
    std::vector<int> monitor = {    0,   0 };
} config;


#ifdef NDEBUG
int WINAPI wWinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prev_instance, _In_ LPWSTR cmd_line, _In_ int cmd_show) {
#else
#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
int main(char* argc, int argv) {
    HINSTANCE instance = GetModuleHandle(0);
#endif

    LoadString(instance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(instance, IDC_SIDEKICK_CLASS, szWindowClass, MAX_LOADSTRING);
    HACCEL hAccelTable = LoadAccelerators(instance, MAKEINTRESOURCE(IDC_SIDEKICK_ACC));

    core::cDisplay display;
    config.monitor[0] = GetSystemMetrics(SM_CXSCREEN);
    config.monitor[1] = GetSystemMetrics(SM_CYSCREEN);
    rect_screen_workarea = { -SM_CXBORDER * 2 + 1, 1,  GetSystemMetrics(SM_CXMAXIMIZED) + 1, GetSystemMetrics(SM_CYMAXIMIZED) - int(SM_CYBORDER * 2) + 2 };
    display.Create(szTitle, szWindowClass, config.screen[0], config.screen[1]);
    display.ogl.SetState();
    
    shader_gui.Create();

    // x, y, z, r, g, b
    float vdata[18] = { -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
                         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
                         0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f 
                      };
    unsigned int idata[3] = { 0, 1, 2 };
    model.Upload(3, 3, 6, vdata, idata);


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
        display.ogl.End();
    }

 
    model.Destroy();
    shader_gui.Destroy();
    
    return (int)msg.wParam;
}


/*/ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Main Window Message Handler - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
LRESULT CALLBACK proc_Wnd(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId;

    switch (message)
    {

    case WM_COMMAND:
        wmId = LOWORD(wParam);
        
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, proc_About);
            break;


        case IDM_RFI:
            break;

        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
*/

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

