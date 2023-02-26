
#include "resource/res.h"
#include "resource/targetver.h"

#include <string>

#include "core/display.h"

// global variables
#define MAX_LOADSTRING 100                 // max array buffer size
HINSTANCE hInst;                           // current instance
char szTitle[MAX_LOADSTRING];              // title bar text
char szWindowClass[MAX_LOADSTRING];        // main window class name

HWND hwnd_parent;
RECT rect_screen_workarea;                 // screen space (x, y, w, h)
RECT rect_client_workarea;                 // client space (0, 0, w, h)

// forward declarations
LRESULT CALLBACK proc_Wnd(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK proc_About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY WinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPSTR lpCmdLine,
                     _In_ int nCmdShow)
{
    hInst = hInstance;
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_SIDEKICK_CLASS, szWindowClass, MAX_LOADSTRING);

    // initialize parent window
    Core::registerWindowClass(szWindowClass, &proc_Wnd, 0);
    rect_screen_workarea = { -SM_CXBORDER * 2 + 1, 1,  GetSystemMetrics(SM_CXMAXIMIZED) + 1, GetSystemMetrics(SM_CYMAXIMIZED) - int(SM_CYBORDER * 2) + 2 };
    hwnd_parent = Core::createWindowObject(nullptr, szWindowClass, Style::eStyle::parent, 0, szTitle, rect_screen_workarea);

    
    // main message processing loop
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SIDEKICK_ACC));
    MSG msg;
       
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
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

        case IDM_INVOICE_NEW:
            break;

        case IDM_INVOICE_LOAD:
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


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Message handler for about box - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
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

