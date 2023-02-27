
#include "resource/targetver.h"
#include "core/display.h"

#include <string>

// global variables
#define MAX_LOADSTRING 100                 // max array buffer size
char szTitle[MAX_LOADSTRING];              // title bar text
char szWindowClass[MAX_LOADSTRING];        // main window class name

HINSTANCE hInst;                           // current instance
HWND hwnd_parent;
RECT rect_screen_workarea;                 // screen space (x, y, w, h)
RECT rect_client_workarea;                 // client space (0, 0, w, h)

// forward declarations
LRESULT CALLBACK proc_Wnd(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK proc_Invoice(HWND, UINT, WPARAM, LPARAM);
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
    hwnd_parent = Core::createWindowObject(nullptr, szWindowClass, Core::eStyle::parent, 0, szTitle, rect_screen_workarea);
    
    
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

        case IDM_INVOICE_NEW:
            //ctrl_inv.reset();
            DialogBox(hInst, MAKEINTRESOURCE(IDD_INVOICEBOX), hWnd, proc_Invoice);
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
// Invoice Control Message Handler - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// https://docs.microsoft.com/en-us/windows/win32/api/winuser/

INT_PTR CALLBACK proc_Invoice(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {

    case WM_INITDIALOG:
        /*if (ctrl_inv.isLoadedRecord() == 1)
        {
            SetDlgItemInt(hDlg, IDC_INV_NUM, ctrl_inv.data.id, false);
            SetDlgItemText(hDlg, IDC_INV_DATE, ctrl_inv.data.date.c_str());
            // fill entity(sold_to) drop down list and select current name     
            for (Data::Entity entity : data_entities)
                SendMessage(GetDlgItem(hDlg, IDC_INV_SOLD_TO_NAME), CB_ADDSTRING, 0, (LPARAM)entity.name.c_str());
            SendMessage(GetDlgItem(hDlg, IDC_INV_SOLD_TO_NAME), CB_SELECTSTRING, 0, (LPARAM)ctrl_inv.data.sold_to_name.c_str());

            SetDlgItemText(hDlg, IDC_INV_SOLD_TO_STREET, ctrl_inv.data.sold_to.street.c_str());
            SetDlgItemText(hDlg, IDC_INV_SOLD_TO_CITY, ctrl_inv.data.sold_to.cityState.c_str());
            SetDlgItemText(hDlg, IDC_INV_SOLD_TO_ZIP, ctrl_inv.data.sold_to.zip.c_str());
            SetDlgItemText(hDlg, IDC_INV_SHIPPED_TO_NAME, ctrl_inv.data.shipped_to_name.c_str());
            SetDlgItemText(hDlg, IDC_INV_SHIPPED_TO_STREET, ctrl_inv.data.shipped_to.street.c_str());
            SetDlgItemText(hDlg, IDC_INV_SHIPPED_TO_CITY, ctrl_inv.data.shipped_to.cityState.c_str());
            SetDlgItemText(hDlg, IDC_INV_SHIPPED_TO_ZIP, ctrl_inv.data.shipped_to.zip.c_str());
            SetDlgItemText(hDlg, IDC_INV_SHIPPED_TO_ATTN, ctrl_inv.data.attn.c_str());
            SetDlgItemText(hDlg, IDC_INV_P_O, ctrl_inv.data.PO.c_str());
            SetDlgItemText(hDlg, IDC_INV_TERMS, ctrl_inv.data.terms.c_str());
            SetDlgItemText(hDlg, IDC_INV_SHIPPED_VIA, ctrl_inv.data.shipped_via.c_str());
            SetDlgItemText(hDlg, IDC_INV_P_C, ctrl_inv.data.p_c.c_str());

            ctrl_inv.resetLineItemsString(hDlg);

        }
        else {
            ctrl_inv.reset();
            // auto fill todays date and next invoice in sequence

            ctrl_inv.data.id = data_config.next_invoice_id;
            SetDlgItemText(hDlg, IDC_INV_DATE, ctrl_inv.data.date.c_str());
            SetDlgItemInt(hDlg, IDC_INV_NUM, ctrl_inv.data.id, false);

            // fill entity(sold_to) drop down list     
            for (Data::Entity entity : data_entities)
                SendMessage(GetDlgItem(hDlg, IDC_INV_SOLD_TO_NAME), CB_ADDSTRING, 0, (LPARAM)entity.name.c_str());
            // misc autofill of edittext fields
            SetDlgItemText(hDlg, IDC_INV_TERMS, "Net30");
        }*/
        return (INT_PTR)TRUE;


    case WM_COMMAND:

        /*if (HIWORD(wParam) == CBN_SELCHANGE) // auto sold to info based on chosen name
        {
            int index = SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
            ctrl_inv.data.sold_to_name = data_entities[index].name;
            ctrl_inv.data.sold_to.street = data_entities[index].address.street;
            ctrl_inv.data.sold_to.cityState = data_entities[index].address.cityState;
            ctrl_inv.data.sold_to.zip = data_entities[index].address.zip;
            SetDlgItemText(hDlg, IDC_INV_SOLD_TO_STREET, data_entities[index].address.street.c_str());
            SetDlgItemText(hDlg, IDC_INV_SOLD_TO_CITY, data_entities[index].address.cityState.c_str());
            SetDlgItemText(hDlg, IDC_INV_SOLD_TO_ZIP, data_entities[index].address.zip.c_str());
        }

        if (LOWORD(wParam) == IDC_INV_TAXABLE)
        {
            ctrl_inv.data.taxable = !ctrl_inv.data.taxable;
            SendMessage(GetDlgItem(hDlg, IDC_INV_TAXABLE), BM_SETCHECK, ctrl_inv.data.taxable ? 1 : 0, 0);
        }

        if (LOWORD(wParam) == IDB_INV_ADD_ENTRY)
        {  // add current edit fields to internal list 
            ctrl_inv.addItemFromFields(hDlg);
        }

        if (LOWORD(wParam) == IDB_INV_PREV_ENTRY)
        {  // decrement or wrap to top
            ctrl_inv.moveItemIndex(-1);
            ctrl_inv.resetLineItemInputFields(hDlg);
        }

        if (LOWORD(wParam) == IDB_INV_NEXT_ENTRY)
        {  // increment or wrap to bottom
            ctrl_inv.moveItemIndex(1);
            ctrl_inv.resetLineItemInputFields(hDlg);
        }

        if (LOWORD(wParam) == IDB_INV_REMOVE_ENTRY)
        {  // internal indexed 
            ctrl_inv.removeIndexedItem(hDlg);
        }

        if (LOWORD(wParam) == IDB_INV_SAVE)
        {
            ctrl_inv.data.id = GetDlgItemInt(hDlg, IDC_INV_NUM, nullptr, false);
            GetDlgItemText(hDlg, IDC_INV_DATE, ctrl_inv.data.date.data(), 16);
            GetDlgItemText(hDlg, IDC_INV_SHIPPED_TO_NAME, ctrl_inv.data.shipped_to_name.data(), 64);
            GetDlgItemText(hDlg, IDC_INV_SHIPPED_TO_STREET, ctrl_inv.data.shipped_to.street.data(), 64);
            GetDlgItemText(hDlg, IDC_INV_SHIPPED_TO_CITY, ctrl_inv.data.shipped_to.cityState.data(), 64);
            GetDlgItemText(hDlg, IDC_INV_SHIPPED_TO_ZIP, ctrl_inv.data.shipped_to.zip.data(), 16);
            GetDlgItemText(hDlg, IDC_INV_SHIPPED_TO_ATTN, ctrl_inv.data.attn.data(), 64);

            GetDlgItemText(hDlg, IDC_INV_P_O, ctrl_inv.data.PO.data(), 64);
            GetDlgItemText(hDlg, IDC_INV_TERMS, ctrl_inv.data.terms.data(), 64);
            GetDlgItemText(hDlg, IDC_INV_SHIPPED_VIA, ctrl_inv.data.shipped_via.data(), 64);
            GetDlgItemText(hDlg, IDC_INV_P_C, ctrl_inv.data.p_c.data(), 64);

            if (ctrl_inv.saveRecord() == true)
            {
                EndDialog(hDlg, LOWORD(wParam));

                char msg[32];
                sprintf_s(msg, 32, "Invoice_%d saved", ctrl_inv.data.id);
                if (MessageBox(0, "Proceed with PDF fomatting?", msg, MB_YESNO) == IDYES)
                    PDF_doc_template::create_invoice(ctrl_inv);

                data_config.next_invoice_id++;
                Loaders::saveConfiguration(data_config);
                return (INT_PTR)TRUE;
            }
            MessageBox(0, "Unable to save current record", "Warning", MB_OK | MB_ICONERROR);
            break;
        }
        */
        if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
    } 
    return (INT_PTR)FALSE;
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

