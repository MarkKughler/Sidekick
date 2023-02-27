#include "display.h"

ATOM Core::registerWindowClass(LPCSTR wndClassName, WNDPROC wndProc, int wndExtra)
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = wndProc;
	wcex.cbWndExtra = wndExtra;
	wcex.hInstance = GetModuleHandle(0);
	wcex.hIcon = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SIDEKICK));
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SIDEKICK_SM));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_SIDEKICK_MENU);
	wcex.lpszClassName = wndClassName;

	return RegisterClassEx(&wcex);
}


HWND Core::createWindowObject(HWND hParent, LPCSTR wndClassName, eStyle style, DWORD ws_style, const char* title, RECT rect)
{
	DWORD out_style = 0;
	DWORD out_style_ex = 0;
	switch (style)
	{
	case eStyle::parent:
		out_style_ex = WS_EX_OVERLAPPEDWINDOW;
		out_style = WS_SYSMENU | WS_VISIBLE | WS_CLIPCHILDREN;
		break;
	case eStyle::ctrl:
		out_style_ex = WS_EX_CLIENTEDGE;
		out_style = WS_CHILD | WS_VISIBLE | ws_style;
		break;
	}

	HWND hwnd = CreateWindowEx(out_style_ex, wndClassName, title, out_style,
		rect.left, rect.top, rect.right, rect.bottom,
		hParent, nullptr, GetModuleHandle(0), nullptr);

	UpdateWindow(hwnd);
	return hwnd;
}


RECT Core::getRectWorkarea()
{
	RECT rect_screen = {};
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rect_screen, 0);

	if (rect_screen.top != 0) rect_screen.bottom -= rect_screen.top;
	if (rect_screen.left != 0) rect_screen.right -= rect_screen.left;
	// todo start bar top
	// todo start bar left
	return rect_screen;
}

